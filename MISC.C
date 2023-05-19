//
//                          MISC.C
//    This module contains the DOF main formula and other bits.
//
//
#define NO_EXT_KEYS // extensions disabled
#include <conio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mylib.h"
#include <easyc.h>             // defines EQ LT GT etc
#include "window.h"
#include "screen.h"
#include "keyboard.h"
#include "dof.h"
#include "globals.h"
#include "menu.h"
#include "status.h"

#define PAUSE_DURATION  5   // time of delay in shareware pause

void    DofCalc( void );    // Calculates near_limit & far_limit
void    PauseWin( void );     // holds things up a while

   //------------------------------------------------------------------
   // Function uses formulae to calculate DOF figures.
   //------------------------------------------------------------------

void DofCalc(void)
{
                     // Hyperfocal length in metres
    hyperfocal =( ( focal_length MUL focal_length )
                DIV ( f_stop MUL cir_conf DIV 1000 ) DIV 1000 );

                     // Near limit of field in metres
    near_limit =( ( hyperfocal MUL focused_distance )
                DIV ( hyperfocal + focused_distance ) );

                     // Far limit of field in metres
    far_limit = ( ( hyperfocal MUL focused_distance )
                DIV ( hyperfocal - focused_distance ) );
}

   //------------------------------------------------------------------
   //       Function delays in unregistered versions.
   //------------------------------------------------------------------

void    PauseWin( void )     // holds things up a while
{
#if NOT defined ( REGISTERED )
    WINDOW      *pause;
#endif

    extern      char registration_string[];

    registration_string[1] = 'ÿ';   // space is alt 255 for later checking

#if NOT defined ( REGISTERED )
    pause = WnMake( 67, 7, 5, 4, V_BLACK, V_CYAN );
    WnBorder( pause, DOUBLE );
    WnPrintxy ( pause, 4, 1, "Registered Versions of the programme do not have this" );
    WnPrintxy ( pause, 3, 2, "screen in them...Please consider registering this copy." );
    WnPrintxy ( pause, 11, 3,        "See the Documentation for details." );
    WnOpen ( pause );

    VsDisp ();
    SPause ( PAUSE_DURATION );  // wait 5 seconds

    WnCls ( pause, 1, 3 );      // clear the old message
    while ( KeyPressed() )      // discards any keys pressed !!!
        GetKey();

    WnPrintxy ( pause, 12, 2, "PRESS ALT + F10 TO CONTINUE..." );
    VsDisp();                   // a new message

    while ( GetKey() NE AF10 )  // wait till alt f10 is hit
        ;

    WnCls ( pause, 2, 1 );    //  be polite
    WnPrintxy ( pause, 15, 2, "THANK YOU ......" );
    VsDisp();

    TPause ( 6 );

    WnKill ( pause );
#endif
}

