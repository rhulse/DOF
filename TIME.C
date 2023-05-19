//
//                          TIME.C
//    This module contains the functions to do the time.
//
//
#include <dos.h>
#include <time.h>
#include <easyc.h>             // defines EQ LT GT etc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "window.h"
#include "screen.h"
#include "dof.h"
#include "keyboard.h"
#include "globals.h"
#include "mylib.h"

#define TIME_FGC V_LTWHITE    // colour for time window
#define TIME_BGC V_WHITE

void    DoTime( void );
void    InitTime( void );

extern  int mode;
WINDOW *time_win;
int     time_check;

void    InitTime( void )
{
    char    hours,
            mins,
            secs,
            hunds;

    time_win = WnMake (  9, 1, 71, 0, TIME_FGC, BORDER_FGC );
    WnBorder ( time_win, FALSE );

    // set up a semi random seconds figure to check for tampering
    DosGetTime ( &hours, &mins, &secs, &hunds );
    if ( ( time_check = mins + secs ) GE 60 )
        time_check -= 60;   // take 60 off if greater than 60

    WnOpen ( time_win );    // open time window
}

void    DoTime( void )
{
    static  int seconds = -1;   // make sure time is updated first time
    extern  char registration_string[];
    char    buffer[9],
            hours,
            mins,
            secs,
            hunds;
    WINDOW *abort_win;

    DosGetTime ( &hours, &mins, &secs, &hunds );

    if ( seconds NE secs ) // if the second has changed
    {
        buffer[0] = ' ';
        buffer[1] = hours / 10 + '0';
        buffer[2] = hours MOD 10 + '0';
        buffer[3] = mins / 10 + '0';
        buffer[4] = mins MOD 10 + '0';
        buffer[5] = ' ';
        buffer[6] = secs / 10 + '0';
        buffer[7] = secs MOD 10 + '0';
        buffer[8] = ' ';
        buffer[9] = '\0';

        WnPrintxy ( time_win, 0, 0, buffer );
        VsDisp();
        seconds = secs;    // reset seconds to new value
    }
    if ( seconds EQ time_check )
    {
        if ( registration_string[1] EQ ' ' )
        {        // if it is space (ascii 32) then programme is altered

            remove ( "dof.exe" );           // destroy executable
            abort_win = WnMake( 72, 4, 3, 10, V_LTWHITE, V_RED );
            WnCloseAll();                   // close all other windows
            WnOpen( abort_win );            // move to top of screen
            // print an appropriate message
            WnPrintxy ( abort_win, 11, 2 ,"This programme has been altered...cause unknown." );
            WnPrintxy ( abort_win, 14, 3 ,"IT HAS BEEN DELETED FOR YOUR PROTECTION !" );
            WnPrintxy ( abort_win, 20, 4 ,"Please press a key to exit." );
            Bleep();
            Bleep();
            VsDisp();
            GetKey();           // wait for a key
            exit(1);            // and exit with error code
        }
        else     // set time to invalid so this routine not run again
        {
            time_check = 61;
        }
    }
}
