//
//                          FILE.C
//      This module contains the functions on the file menu.
//
//
#define NO_EXT_KEYS // extensions disabled
#include <sys\types.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <malloc.h>
#include <alloc.h>
#include <string.h>
#include <easyc.h>             // defines EQ LT GT etc
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <dos.h>
#include <io.h>
#include "mylib.h"
#include "window.h"
#include "screen.h"
#include "keyboard.h"
#include "dof.h"        // defines menu and help strings
#include "globals.h"

BOOL    CopyFile( char * );
int     DoCopy( char *, char * );
void    MakeDest ( char *, char *, char * );
int     CheckIfSame ( char *, char * );
long    GetSpace ( char * );
BOOL    MoveFile( char * );
int     DoMove( char *, char * );
BOOL    DeleteFile( char * );

extern int critical_error;

   //-------------------------------------------------------------------
   //   Copy function
   //-------------------------------------------------------------------

BOOL CopyFile( char *source )
{
    WINDOW *copy_win;
    char input[ 64 ];

    strcpy ( input , "Copy " );
    strcat ( input , source );

    if ( GetDest( input ) AND ( input[0] NE '\0' ) )
    {
        copy_win = Message( "Copying File....", MESSAGE_FGC, MESSAGE_BGC );
        VsDisp();
        
        if ( DoCopy ( source, input ) )
            Error();

        WnKill ( copy_win );
    }
    return ( TRUE );
}

   //-------------------------------------------------------------------
   //  This module does the actual copying
   //-------------------------------------------------------------------

int DoCopy( char *source_name, char *dest_name )
{
    struct dta source_info;
    char    new_dest[ _MAX_PATH ],
            *buffer;               // buffer for reads and writes
    int     source,                // handles for files
            dest,
            error_flag = FALSE;
    WORD    bytes_read,
            heap_size,
            buffer_size,
            file_size;

    if ( ( source = open ( source_name, O_BINARY | O_RDONLY ) ) EQ -1)
        error_flag = TRUE;
    else
    {
        DosFindFirst ( source_name, 0, &source_info );

        // make a destination that DOS understands !!
        MakeDest ( new_dest, source_name, dest_name );

        if ( CheckIfSame ( source_name, new_dest ) EQ TRUE )
            error_flag = TRUE;
        else
        {
            if ( GetSpace ( new_dest ) LE source_info.size )
                error_flag = TRUE;
            else
            {
                if ( ( dest = open ( new_dest, O_WRONLY | O_BINARY | O_TRUNC | O_CREAT, S_IWRITE )) EQ (int)-1 )
                    error_flag = TRUE;
                else
                {
                    file_size = (unsigned int)filelength ( source );
                    heap_size = coreleft();
                    buffer_size = ( file_size LT heap_size ) ? file_size : heap_size;

                    if ( ( buffer = malloc ( buffer_size ) ) EQ NULL )
                    {
                        AllocError();   // terminate programme..does not return
                    }

                    else while ( NOT eof( source ) )
                    {
                        if ( ( bytes_read = read( source, buffer, buffer_size ) ) LE 0 )
                            error_flag = TRUE;
                        else if ( write( dest, buffer, bytes_read) NE bytes_read )
                            error_flag = TRUE;
                    }
                    if ( error_flag EQ FALSE )
                        _dos_setftime ( dest, source_info.wr_date, source_info.wr_time );

                    free ( buffer );
                    close ( dest );
                }
            }
        }
        close ( source );   // close only if it was opened
    }
    return ( error_flag );
}

   //-------------------------------------------------------------------
   // Make a valid DOS destination from the entered dest and the source
   //-------------------------------------------------------------------

void MakeDest ( char *new_dest, char *source, char *dest )
{
    struct dta dest_info;
    int     n;

    strcpy ( new_dest, dest );
    n = strlen( new_dest ) - 1 ;

    if ( new_dest[n] EQ '\\' )   // if it is already is a full path
    {
        strcat ( new_dest, source );   // attach file name
    }
    else if ( DosFindFirst ( dest, _A_SUBDIR, &dest_info ) EQ 0 )
    {
        strcat ( new_dest, "/" );      // else attach '/' then filename
        strcat ( new_dest, source );
    }
}

   //-------------------------------------------------------------------
   // Gets the amount of free space on the targets disk
   //-------------------------------------------------------------------

long GetSpace ( char *dest )
{
    int     disk ;
    long    bytes_free;
    struct diskfree_t disk_info;

    if ( strlen ( dest ) GE 2 AND dest[1] EQ ':' )
        disk = ( toupper ( dest[0] ) ) - 'A' + 1 ;
    else
        disk = 0;  // the default

    _dos_getdiskfree ( disk, &disk_info );

    bytes_free = (long) disk_info.avail_clusters *
                 (long) disk_info.sectors_per_cluster *
                 (long) disk_info.bytes_per_sector;

    return ( bytes_free );
}

   //-------------------------------------------------------------------
   //     Checks to see if the source and target are the same file !!
   //-------------------------------------------------------------------

int CheckIfSame ( char *source, char *dest )
{
    int     source_attr,
            dest_attr,
            temp,
            check = FALSE;

    if ( NOT _dos_getfileattr ( source, &source_attr ) )
    {
        if ( NOT _dos_getfileattr ( dest, &dest_attr ) )
        {
            _dos_setfileattr ( dest, dest_attr BXOR 1 );
            _dos_getfileattr ( source, &temp );
            if ( temp NE source_attr )
                check = TRUE;
            _dos_setfileattr ( dest, dest_attr );
        }
    }
    if ( critical_error )
        check = TRUE;

    return ( check );
}

   //-------------------------------------------------------------------
   //    Move function
   //-------------------------------------------------------------------
BOOL MoveFile( char *source )
{
    WINDOW *move_win;
    char dest[ 64 ] ;

    strcpy ( dest , "Move " );
    strcat ( dest , source );

    if ( GetDest( dest ) AND ( dest[0] NE '\0' ) )
    {
        move_win = Message( "Moving File....", MESSAGE_FGC, MESSAGE_BGC );
        VsDisp();

        if ( DoMove ( source, dest ) )
            Error();

        WnKill ( move_win );
    }
    return ( TRUE );
}

   //-------------------------------------------------------------------
   //  This one does the actual move
   //-------------------------------------------------------------------

int DoMove ( char *source, char *dest )
{
    char new_dest[ _MAX_PATH ];
    int error_flag = FALSE;

    MakeDest ( new_dest, source, dest );

    if ( access ( new_dest, 0 ) NE -1 ) // returns -1 if does not exist
        error_flag = TRUE;

    else if ( rename ( source, new_dest ) NE 0 ) // returns 0 if renamed
        error_flag = TRUE;

    return ( error_flag );
}

   //-------------------------------------------------------------------
   //          Delete function
   //-------------------------------------------------------------------

BOOL DeleteFile( char *source )
{
    WINDOW *del_win,
           *question;
    int     entry;
    char    temp[30];

    strcpy ( temp , "Delete " );
    strcat ( temp , source );
    strcat ( temp , " ?  Y or N" );

    question = Message( temp, WARN_FGC, WARN_BGC );
//    WnBorderChg ( question, FALSE );   // dont change border when active
    VsDisp();

    entry = (char) ( GetAKey() BAND 0x00FF );

    WnKill ( question );

    if ( isalpha ( entry ) AND ( toupper ( entry ) EQ 'Y') )
    {
        del_win = Message( "Deleting File....", WARN_FGC, WARN_BGC );
        VsDisp();

        if ( remove ( source ) )
            Error();
        else
            source[0] = '\0';       // set so that `view' knows to quit

        WnKill ( del_win );
    }
    return ( TRUE );
}
