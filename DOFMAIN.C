//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
//     DOF   Version 1.10   Written   for Quick C V 1.01b                   *
//                      DOFMAIN.C                                           *
//                            1 August 1991                                 *
//                                                                          *
//             This version uses the window module.                         *
//            It was modularised on the 18 OCT 1990                         *
//            This Programme calculates the Hyperfocal Length               *
//            and Depths of fields for specified Focal lengths              *
//            and various apertures.                                        *
//                                                                         *
//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
#define NO_EXT_KEYS     // extensions disabled
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <easyc.h>      // defines EQ LT GT etc
#include "mylib.h"
#include "window.h"
#include "screen.h"
#include "dv.h"
#include "globals.h"
#include "keyboard.h"
#include "dof.h"        // defines menu and help strings
#include "menu.h"
#include "help.h"

void    Args2 ( char ** );
int     GetAKey( void );
void    Bleep( void );
extern  void    Init( void );       // does initialisations
void    DoResets( void );           // restore old screenand cursor

#if defined ( BETA )

void    Beta( void );

#endif

#if defined ( REGISTERED )

int registered = TRUE;
char registration_string[] =
{"  *** UNREGISTERD BETA TEST VERSION ***"};

// { "  Registered to Richard Hulse" };

#else

int registered = FALSE;
char registration_string[] =
{"  *** UNREGISTERD COPY ***"};

#endif

// registration_string[1] is changed to alt 255 for tamper checking
// therefore there MUST be two leading spaces in the string.

//
//  values used by whole dof programme
//
float   focal_length = (float)50, //  Default Focal Length
        f_stop = (float)4.0,      //  Default F Stop
        cir_conf = (float)33,     //  Default Alowable circle of confusion in ๆm
        hyperfocal,               //  Hypefocal Distance
        near_limit = (float)0,    //  some more globals
        far_limit = (float)0,     //  some more globals
        focused_distance = (float)10; //  Default Focused Distance

extern  MENU MAIN_MENU;

int error_flag = FALSE;         // an error in one of the parameter entries
extern  int     video_method;   // use direct screen writes
extern  int     snow_free;      // do we do snow_free writes on CGA
extern  int     mono_attr;      // force monochrome attributes
extern  int     mode_change;    // force monochrome attributes
extern  int     col_shadow;

void main(int argc, char *argv[])
{
    snow_free = FALSE;      // dont do snow free updates unless the user wants to
    mode_change = FALSE;    // if false then use any 80 x 25 text mode

    switch ( argc )
    {
        case 1 : Init();

#if defined ( BETA )
                 Beta ();
#endif
                 RunPullDownMenu();     //  run the whole programme !!!
                 break;

        case 4 : CommandLine( argv ); break;

        default : CLHelp(); break;
    }
    DoResets();
    exit(0);
}

int GetAKey( void )
{
    while ( NOT KeyPressed() )   // if no key ready then do time
    {
        DoTime();               // keep the clock running
        DvPause();              // tell DESQview to slice away
    }
    return ( GetKey() );
}

void    Bleep( void )
{
    Beep ( 500, 1 );  Beep ( 1500, 1 );  Beep ( 1000, 1 );
}

//------------------------------------------------------------------
//               Function cleans up on exit
//------------------------------------------------------------------

void DoResets(void)
{
    WnRestoreInt24();       // Restore int 24...not really necesary
    WnExit();               //  clean up window module

#if defined( REGISTERED )   // save a little space if they've payed !!
        cputs( "\r\nThank You for registering your copy of DOF"
                "...your support is appreciated.\n\r");
#else
    cputs( "\n\r                     DOF version 2.00  August 1992");
    cputs( "\n\r   Copyright 1985-1992 by Riverside Developments, All Rights Reserved");
    cputs( "\n\r");
    cputs( "\n\r         ษออออออออออออออออออออออออออออออออออออออออออออออออออออออป");
    cputs( "\n\r         บ      This is NOT a registered copy of DOF.           บ");
    cputs( "\n\r         บ It is a shareware copy for EVALUATION PURPOSES ONLY. บ");
    cputs( "\n\r         ศออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");
    cputs( "\n\r     If you continue using DOF beyond a 30-day trial period, you");
    cputs( "\n\r     MUST PURCHASE a registered version of the programme. You will");
    cputs( "\n\r     recieve free technical support and low-cost updates.");
    cputs( "\n\r");
    cputs( "\n\r            DOF version 2.0 is $NZ 40 including P & P.");
    cputs( "\n\r");
    cputs( "\n\r       Riverside Developments             PHONE: (644) 569-7353");
    cputs( "\n\r       63 Riverside Drive,");
    cputs( "\n\r       LOWER HUTT  6301,");
    cputs( "\n\r       NEW ZEALAND.\r\n");
#endif
}

#if defined ( BETA )
void    Beta( void )
{
    WINDOW *beta;
    extern  char *title_string[];

    beta = WnMake( 60, 6, 9, 10, V_LTBLUE , V_BLACK );
    WnBorderCol ( beta, V_YELLOW , V_BLACK ); // change colour of window border

    WnPrintxy ( beta, 2, 1, title_string[0] );
    WnPrintxy ( beta, 2, 2, title_string[1] );
    WnPrintxy ( beta, 4, 4, " BETA TEST VERSION...NOT FOR DISTRIBUTION" );
    WnOpen ( beta );
}
#endif

