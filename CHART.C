//
//                          CHART.C
//    This module contains the functions to make DOF charts.
//
//
#define NO_EXT_KEYS // extensions disabled
#include <io.h>          
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <process.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <easyc.h>             // defines EQ LT GT etc
#include "window.h"
#include "keyboard.h"
#include "dof.h"        // defines menu and help strings
#include "globals.h"
#include "status.h"

void    ChartDofs( void );
void    MakeChart( float *, int * );
char   *MakeDofFileName( char * );
void    MakeTitleLines( char *, char *, char * );
char   *MakeTopLine( int *, int, char * );
int     MakeResultLines( int *, int, char *, char * );
char   *MakeHDLine( int *, int, char * );
int     CheckFile ( char * );
char   *FindReplace( char *, char, char );

char    spaces2[] = "  ",
        spaces3[] = "   ",
        spaces4[] = "    ",
        spaces5[] = "     ",
        spaces8[] = "        ",
        spaces13[] = "             ",
        file_ext[] = ".DOF";      //  Default extension

       int	max_width = 10;

extern  char    *title_string[];
extern  char    file_menu_letters[];
extern  int     file_menu_hilites[];

// all fstops are stored as int by multiplying by 10 the
// calling routine divids them by 10..this save data space

int whole_f_stops[] = { 10, 14, 20, 28, 40, 56, 80, 110,
                      160, 220, 320, 450, 640, 910, 1280, -1 };

int half_f_stops[] = { 10, 12, 14, 17, 20, 24, 28, 34, 40,
                       47, 56, 68, 80, 95, 110, 135, 160,
                       190, 220, 270, 320, 380, 450, 540,
                       640, 760, 910, 1080, 1280, -1 };

int third_f_stops[] = { 10, 11, 13, 14, 16, 18, 20, 22, 25,
                        28, 32, 36, 40, 45, 50, 56, 64, 71,
                        80, 90, 100, 110, 130, 140, 160, 180,
                        200, 220, 250, 290, 320, 360, 400,
                        450, 510, 570, 640, 720, 810, 910,
                        1020, 1140, 1280, -1 };

float general_distances[] = { (float) 0.02, (float) 0.05, (float) 0.07,
                              (float) 0.08, (float) 0.10, (float) 0.20,
                              (float) 0.30, (float) 0.45, (float) 0.50,
                              (float) 0.60, (float) 0.80, (float) 1.00,
                              (float) 1.50, (float) 2.00, (float) 3.00,
                              (float) 5.00, (float) 7.50, (float) 10.00,
                              (float) -1 };

float macro_distances[] = { (float) 0.01, (float) 0.02, (float) 0.05,
                            (float) 0.08, (float) 0.10, (float) 0.15,
                            (float) 0.20, (float) 0.30, (float) 0.40,
                            (float) 0.50, (float) 0.60, (float) 0.70,
                            (float) 0.80, (float) 0.90 ,(float) 1.00,
                            (float) -1 };

float tele_distances[] = { (float) 10, (float) 20, (float) 50, (float) 70,
                           (float) 80, (float) 100,(float) 200,(float) 300,
                           (float) 500,(float) 1000, (float) -1 };

   //------------------------------------------------------------------
   //    Function decides which f stop and distancs list to use.
   //------------------------------------------------------------------

void ChartDofs( void )
{
    float *dist;
    int *fs;

    switch ( fstop_list )
    {
        case 0 : fs = whole_f_stops; break;

        case 1 : fs = half_f_stops; break;

        case 2 : fs = third_f_stops; break;
    }
    switch ( distances_list )
    {
        case 0 : dist = general_distances; break;

        case 1 : dist = macro_distances; break;

        case 2 : dist = tele_distances; break;
    }
    MakeChart ( dist, fs );
}

   //------------------------------------------------------------------
   //       Function prints the results
   //------------------------------------------------------------------

void MakeChart( float *distances, int *fstops )
{
    FILE *file;
    char    line_1[150],
            line_2[150],
            line_3[150],
            file_name[14];
    int     x = 0,
            y = 0,
            count = 0;

            // if file check is ok ie. TRUE then proceed

    if ( CheckFile ( MakeDofFileName( file_name ) ) )
    {
        file = fopen ( file_name, "w" );   // open the file

        MakeTitleLines( line_1, line_2, line_3 );
        fprintf( file, "%s\n%s\n%s\n\n", line_1, line_2, line_3 );

        

             // print first ( f stops ) line

        fprintf( file, "%s\n%s\n\n" ,
        MakeTopLine( fstops, y, line_1 ) ,
        MakeHDLine( fstops, y, line_2 ) );

        while ( fstops[y] NE -1 )
        {
#if defined( REGISTERED )   // don't do time in unregistered versions as we
                DoTime();   // dont want tamper checking with files open !!
#endif

            if ( distances[x] NE (float) -1 )
            {
                focused_distance = distances[x];

                count = MakeResultLines( fstops, y, line_1, line_2 );
                fprintf( file, "%s\n%s\n\n", line_1, line_2 );
                x++;
            }
            else
            {
                x = 0;
                y += count;
                if ( fstops[y] NE -1 )
                    fprintf( file, "%c\n%s\n%s\n\n", '' ,
                    MakeTopLine( fstops, y, line_1 ) ,
                    MakeHDLine( fstops, y, line_2 ) );
            }
        }
        fprintf( file, "" );  // put a form feed at the end
        fclose ( file );        // close the file
    }
}

   //-------------------------------------------------------------------
   //  This makes the top line of focal length and circle etc.
   //-------------------------------------------------------------------

void MakeTitleLines( char *title, char *copywrite, char *buffer )
{
	char temp[20];
	extern char *title_string[];

	strcpy ( title, title_string[0] );
	strcpy ( copywrite, title_string[1] );
	sprintf ( buffer, "Focal Length = %3.0f        Maximum Size Of Circle Of Confusion = %s" , focal_length , CreateCC( temp ) );
}

   //-------------------------------------------------------------------
   //  This makes the top line of f stops for the DOF file.
   //-------------------------------------------------------------------

char *MakeTopLine( int *fsl, int y, char *line )
{
    char    buffer[30];
    int     n,
            width_count = 0;

    strcpy ( line, spaces4 ); // 4 spaces at the start of the next line

    while ( fsl[y] NE -1 AND width_count LT max_width )
    {
        strcat ( line, spaces5 );  // 5 more spaces

        f_stop = (float)fsl[y] DIV 10;
        gcvt ( f_stop, 4, buffer );

        // is last char a '.'...replace with a space
            // if greater than ten else add a 0
        n = strlen( buffer ) - 1;
        if ( buffer[ n ] EQ '.' )
        {
            if ( f_stop GE (float)10 )
                buffer[n] = ' ';
            else
                strcat ( buffer, "0" );
        }
        strcat ( line, buffer );
        y++;
        width_count++;
    }
    return ( line );
}

   //-------------------------------------------------------------------
   //  This makes the pair of lines for each distance.
   //-------------------------------------------------------------------

int MakeResultLines( int *fsl, int y, char *l1, char *l2 )
{
    int width_count = 0;
    char buffer[30];

    
    strcpy ( l1, FloatToDist ( focused_distance, buffer ) );
    strcpy ( l2, spaces2 );
    strcat ( l2, spaces3 );

    while( fsl[y] NE -1 AND width_count LT max_width )
    {
        f_stop = (float)fsl[y] DIV 10;
        DofCalc();
        
        strcat ( l1, spaces2 );
        strcat ( l1, FloatToDist ( near_limit, buffer ) );

        strcat ( l2, spaces2 );
        if ( ( far_limit LE (float)0 ) OR ( far_limit GT focal_length ) )
            strcat ( l2, "  I  " );
        else
        {
            strcat ( l2, FloatToDist ( far_limit, buffer ) );
        }
        y++;
        width_count++;
    }
    return ( width_count );
}


char *MakeHDLine( int *fsl, int y, char *l1 )
{
    int width_count = 0;
    char buffer[30];

    strcpy ( l1, "HyDis" );

    while( fsl[y] NE -1 AND width_count LT max_width )
    {
        f_stop = (float)fsl[y] DIV 10;
        DofCalc();
        

        strcat ( l1, spaces2 );

        if ( ( hyperfocal LE (float)0 ) OR ( hyperfocal GT focal_length ) )
            strcat ( l1, "  I  " );
        else
            strcat ( l1, FloatToDist ( hyperfocal, buffer ) );
        y++;
        width_count++;
    }
    return ( l1 );
}

   //-------------------------------------------------------------------
   //  This creates and opens the DOF file and does the title line.
   //-------------------------------------------------------------------

char *MakeDofFileName( char *file_name )
{
    char    buffer1[30],
            buffer2[30];
    int     n = 0;

    // Create xxx_yyy.DOF type file name. xxx is focal length
    // any decimal points in the focal length are replaced with an x.
    // eg 50.3 with circle of confusion of 33 becomes 50x3_33.DOF

    // create focal length text

    gcvt ( focal_length, 5, buffer1 );
    n = strlen( buffer1 ) - 1;

    if ( buffer1[n] EQ '.' )   // if last char is '.' then remove it
        buffer1[n] = '\0';
    else                      // otherwisw replace '.' in string with 'x'.
        FindReplace( buffer1, '.', 'X' ); // replace the '.' with 'X'

    strcpy ( file_name, buffer1 );      // add focal length text
    strcat ( file_name, "_" );         // add an underscore

    // make circle of confusion extension

    gcvt ( cir_conf, 5, buffer2 );
    n = strlen( buffer2 ) -1;

    if ( buffer2[n] EQ '.' )   // if last char is '.' then remove it
        buffer2[n] = '\0';
    else                      // otherwisw replace '.' in string with 'x'.
        FindReplace( buffer1, '.', 'X' ); // replace the '.' with 'X'

    strcat ( file_name, buffer2 );      // add c of c text
    strcat ( file_name, file_ext );   // add DOF file extension

    return ( file_name );
}

   //-------------------------------------------------------------------
   //    Check to see if the file exists.
   //-------------------------------------------------------------------

int CheckFile (  char *file_name )
{
    return ( TRUE );
}

   //-------------------------------------------------------------------
   //    Get the file that node is pointing at ( highlighted )
   //-------------------------------------------------------------------

char *FindReplace( char *buffer, char find, char replace )
{
    char    *ptr;

    ptr = strchr ( buffer, find );  // find the char
    if ( ptr NE NULL )
            *ptr = replace;          // if found then replace

    return ( buffer );
}
