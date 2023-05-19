//
//                    CONSOLE.C
//    This module contains functions that have anything to do with the
//            screen or keyboard.
//
//
#define NO_EXT_KEYS // extensions disabled
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <math.h>
#include <easyc.h>             // defines EQ LT GT etc
#include "keyboard.h"
#include "window.h"
#include "screen.h"
#include "help.h"
#include "dof.h"        // defines menu and help strings
#include "globals.h"

float   GetInput( WINDOW *, int, int, float, float );
float   CheckInput( char *, float, float ); // checks the focal length input
int     PromptForValues( int );  // gets values for programme
void    PrintSingle( void );      // print single results of calculation
char   *FloatToDist ( float, char * );
void    MakeResultStrings( char *, char *, char * );
void    CommandLine( char ** );
WINDOW *Message( char *, int, int );
int     GetDest( char * );

float   fl_min = (float)5.0,
        fl_max = (float)2000.0,
        fs_min = (float)1.0,
        fs_max = (float)128.0,
        fd_min = (float)0.001,
        fd_max = (float)500.0;

char    um_string[] = " æm",
        mm_string[] = " mm",
        m_string[] = " metres";

extern	 int	 error_flag;
extern	 WINDOW  *menu_win;
WINDOW  *input_win;
WINDOW	*result_win;

   //------------------------------------------------------------------
   //   Function gets a string and echos it to the screen.
   //------------------------------------------------------------------

float GetInput( WINDOW *ptr, int x, int y, float min, float max )
{
    char    buffer[] = "          ";
    float   temp;

/*
for some unknown reason the following line destorys the stack pointer
and the programme has nowhere to return to !!!!!!!!!!!
buffer above initialised to counter this...
    buffer[10] = '\0';
    strset ( buffer, ' ' );
*/

    if ( DWnEdit ( ptr, 5, x, y, buffer ) )
    {
        if( strncmp ( buffer,  "     ", 5 ) NE 0) // check for a NULL
        {                                    // string...FATAL !!!
            temp = (float)atof ( buffer );
            if ( temp GT max OR temp LT min )
            {
                Bleep();
                if ( temp LT min )
                    temp = min;
                else
                    temp = max;
                
                WnPrintxy ( ptr, x, y, FloatToDist ( temp, buffer ) );
            }
        }
    }
    else
        temp = (float)FAIL;
    return ( temp );
}

   //------------------------------------------------------------------
   //       Function checks for valid command line input
   //------------------------------------------------------------------

float CheckInput( char *input, float min, float max )
{
    float temp;

    error_flag = FALSE;
    if ( ( input[0] NE '\0') OR ( input[0] NE ' ') )
        temp = (float)( atof ( input ) );

    if ( ( temp LE (float)0 ) OR ( temp LT min ) OR ( temp GT max ) )
        error_flag = TRUE;
    return ( temp );
}


   //------------------------------------------------------------------
   //  Function prompts for appropriate vaues depending on the copt.
   //------------------------------------------------------------------

int PromptForValues( int number )
{
    int temp = 0;

    input_win = WnMake ( 49, 5, 14, 8, WIN_FGC, WIN_BGC );  // A win for the prompts.
    WnOpen ( input_win );

    WnPrintxy( input_win , 1, 1, "Enter the required focal length :" );
    if ( ( focal_length = GetInput( input_win, 35, 1, fl_min, fl_max ) ) EQ (float)FAIL )
        temp = FAIL;
    else
    {
        if( number EQ TRUE )
        {
            WnPrintxy( input_win, 1, 2, "Enter the set f_stop :" );
            if ( ( f_stop = GetInput( input_win, 24, 2, fs_min, fs_max ) ) EQ (float)FAIL )
                temp = FAIL;
            else
            {
                WnPrintxy( input_win , 1, 3, "Enter the focused distance in metres :" );
                if ( ( focused_distance = GetInput( input_win, 40, 3, fd_min, fd_max ) ) EQ (float)FAIL )
                    temp = FAIL;
            }
        }
    }
    VsDisp();
    return( temp );
}

   //------------------------------------------------------------------
   //          Function prints results for single mode
   //------------------------------------------------------------------

void PrintSingle(void)
{
    char    hyp[70],        // scratch areas to assemble strings
            nlim[70],
            flim[70];

    result_win = WnMake( 49, 5, 14, 8, WIN_FGC, WIN_BGC );

    MakeResultStrings( hyp, nlim, flim );

    WnPrintxy( result_win, 1, 1, hyp );
    WnPrintxy( result_win, 1, 2, nlim );
    WnPrintxy( result_win, 1, 3, flim );
    WnPrintxya( result_win, 8, 4, "Press a key to continue.", V_WHITE, V_LTBLUE );

    WnOpen ( result_win );

    VsDisp();
    GetAKey();       // wait for a keypress
    WnKill ( input_win );
    WnKill ( result_win );
    VsDisp();
}

   //------------------------------------------------------------------
   // Function converts a floating value to a string with correct tale
   //------------------------------------------------------------------

char   *FloatToDist ( float dist, char *dist_string )
{
    int     n = 0,
            no_of_places = 3,
            decptr,
            signptr;
    char    *ptr ;

    if ( dist LE (float)0.001 )
    {
        dist = (float)0.001;       // less than 1 mm is useless !!!!!
    }

    ptr = fcvt ( dist, no_of_places, &decptr, &signptr );

    if ( decptr LE 0 )
    {
        strcpy ( dist_string, "0." );
        if ( decptr LT 0 )
        {
            while ( decptr++ LT 0 )
                strcat ( dist_string, "0" );
        }
        strcat ( dist_string, ptr );
    }
    else if ( decptr GT 0 )
    {
        while ( decptr-- )
            dist_string[n++] = ptr[n];
        dist_string[n] = '.';
        while ( ptr[n] )
            dist_string[n++ + 1] = ptr[n];
    }

// append a null to keep string size at a 5 character length
    if ( dist_string[4] EQ '.' ) // stops a '.' being the last character
        dist_string[4] = ' ';
    dist_string[5] = '\0';

    return ( dist_string ); // return a pointer to the string
}

   //------------------------------------------------------------------
   //     Function creates the strings that give the answers.
   //------------------------------------------------------------------

void MakeResultStrings( char *hstr, char *nstr, char *fstr )
{
    static  char    hyp_str[] = "The hyperfocal distance is ",
                    nl_str[] = "The near limit of the field is ",
                    fl_str[] = "The far limit of the field is ",
                    inf_str[] = "at infinity.";
    char    str[10];

    // hyperfocal distance

    strcpy ( hstr, hyp_str );
    strcat ( hstr, FloatToDist ( hyperfocal, str ) );

    strcat ( hstr, m_string );

    // near depth of field distance

    strcpy ( nstr, nl_str );
    
    strcat ( nstr, FloatToDist ( near_limit, str ) );
    strcat ( nstr, m_string );

    // far depth of field distance

    strcpy ( fstr, fl_str );
    if ( ( far_limit LE (float)0 ) OR ( far_limit GT focal_length ) )
    {                                                                                    
        strcat ( fstr, inf_str );
    }
    else
    {
        
        strcat ( fstr, FloatToDist ( far_limit, str ) );
        strcat ( fstr, m_string );
    }
}

   //------------------------------------------------------------------
   //Function does the command line processing...No direct access..SLOW
   //------------------------------------------------------------------

void CommandLine( char **argv )
{
    char    temp[70],
            hyp[70],
            nlim[70],
            flim[70];
    extern  char *title_string[];

    focal_length = CheckInput( argv[1], fl_min, fl_max );
    f_stop = CheckInput( argv[2], fs_min, fs_max );
    focused_distance = CheckInput( argv[3], fd_min, fd_max );

    if ( error_flag EQ TRUE )
    {
        Bleep();
        printf ( "\nOne of you entries was too big, too small or wasn't a number at all!\n\n" );
        CLHelp();
    }
    else
    {
        // prepare strings for a big printf !
        DofCalc();
        MakeResultStrings( hyp, nlim, flim );

        printf( "%s\n%s\n\nThe Focal Length of the lens is %g%s.\n\
                The set F Stop is f %g.\n\
                The Distance where the lens is focused is %s%s\n\n%s\n%s\n%s\n" ,
                  title_string[0],             // title line 1
                  title_string[1],             // title line 2
                  focal_length, mm_string,    // The Focal Length
                  f_stop,                      // The set F Stop
                  FloatToDist ( focused_distance, temp ), // The Distance
                  m_string,
                  hyp ,
                  nlim ,
                  flim );
    }
}

   //-------------------------------------------------------------------
   //    displays a message.
   //-------------------------------------------------------------------

WINDOW *Message ( char *string, int fgc, int bgc  )
{
    WINDOW *win;

    win = WnMake ( 53, 1, 13, 11, fgc, bgc );

    WnPrintxy ( win, 2, 0, string );
    WnOpen ( win );
    return ( win );
}
   //-------------------------------------------------------------------
   //   Get a destination from the user
   //-------------------------------------------------------------------

int GetDest( char *buffer )
{
//
//  initially the buffer contains the name of the file to be acted on
//
    int     result = FALSE,
            edit_start;
    WINDOW *get_dest_win;

    strcat  ( buffer , " to ->" );
    edit_start = strlen ( buffer ) + 2;

    get_dest_win = Message ( buffer, MESSAGE_FGC, MESSAGE_BGC );
    VsDisp();

    WnOpen ( get_dest_win );

    strncpy ( buffer, " ", 64 );    //  clear the buffer with spaces
//
//  start editing after the prompt and won't let it go outside the window.
//
    result = DWnEdit ( get_dest_win, 53 - edit_start, edit_start , 0, buffer );

    WnKill ( get_dest_win );
    return ( result );
}
