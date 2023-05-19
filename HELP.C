//
//                      HELP.C
//      This module contains all the bits for helping the user !!!!
//
//
#define NO_EXT_KEYS // extensions disabled
#include <conio.h>
#include <stdio.h>
#include <easyc.h>             // defines EQ LT GT etc
#include "window.h"
#include "mylib.h"
#include "screen.h"
#include "keyboard.h"
#include "dof.h"        // defines menu and help strings
#include "menu.h"
#include "globals.h"

#define ABOUT_FGC       WHITE
#define ABOUT_BGC       BLACK
#define ABOUT_EDGE_COL  YELLOW

#define LOGO_START      12
#define PAUSE_TICK      3

BOOL    Help( MENU * );         // General help screen for inside programme
void    CLHelp ( void );        // command line help
void    DoHelp ( WORD );      // does help
BOOLEAN AboutDof ( void );

char   *title_string[] = {
    "   DOF Depth Of Field Calculator for Photographers.",
    "Version 2.00 (C)opyright 10 Oct 1992 by Richard Hulse.",
    " Use the arrow keys and then ENTER to make a selection.",
    "           F1 = Help           <ALT> Q = EXIT" };


// the help strings

char *help_str0[] = { "No help is available." };

char *help_str1[] =
{    // 15 lines
    "DOF works out the depth of field based on Focal Length, F Stop, and the",
    "distance where the lens is focused. You can run DOF from the command ",
    "line or type DOF for a menu driven interface.",
    " ",
    " Syntax :  DOF [focal_length] [f_stop] [distance]   eg. DOF 50 4 3.5 ",
    "Focal Length range = 5 to 2000 mm.",
    "F Stop range       = f1.0 to f128.",
    "Distance range     = 0.001 to 500 m.",
    " ",
    "The status line at the bottom of the title box tells you which data",
    "lists DOF will use for its calculations. The left hand field shows the",
    "list of distances DOF uses when it makes a chart. The right field",
    "shows the F stop list for same. The centre field shows the size of the",
    "maximum allowable circle of confusion that is used for all DOF",
    "calculations."
};

char *help_str2[] = { "Select this Option or hit <ALT> Q to get out of DOF." };

char *help_str3[] = {
    " If you select this option then you will be prompted for the",
    "Focal Length, F Stop and the Distance where the lens is focussed.",
    " DOF will provide the Hyperfocal distance for that F Stop and ",
    "the near and far limits of the field of sharpness based on the",
    "default maximum allowable circle of confusion." };

char *help_str4[] = {
    " If you select this option you will be prompted for a",
    "Focal Length only. DOF will create a file called DOFxx.DAT",
    "which will have a chart of various depth of fields based on a",
    "range of distances AND f stops. xx is the focal length." };

char *help_str5[] = {
    "This option allows you to change the maximum allowable",
    "Circle of Confusion on which DOF bases its calculations",
    "The Default is 33 micrometres which is for the 35 mm",
    "format. The allowable range is 1 to 1000 цm.(.001 - 1mm)"};

char *help_str6[] = {
    " This option lets you change the F Stop",
    "list DOF uses for making its charts." };

char *help_str7[] = {
    " This option lets you change the list of",
    "focussed distances DOF uses for it charts." };

char *help_str8[] = { "The Distances used are from 0.01 to 10 meters." };

char *help_str9[] = { "The Distances used are less than 1 metre." };

char *help_str10[] = { "The Distances used are greater than 10 meters." };

char *help_str11[] = {
    "Use the the Up and Down arrow keys, Page Up, Page Down, Home or End",
    "to move through the file. The highlight bar can be moved with",
    " the left and right arrow keys.",
    "The following functions can be accessed from within the file viewer..",
    "Copy = ALT + C , Move = ALT + M , Delete = ALT + D , Print = ALT + P"};

char *help_str12[] = {
    "xxx_yyy.DOF",
    " Г  Г    Р Simply means it is a DOF file.",
    " Г  Г",
    " Г  Р yyy is the circle of confusion",
    " Г     that file is based on.",
    " Г",
    " Р xxxis the focal length that",
    "    the file is based on." };

   // an array of HELP structures.

struct HELPER
{
    int wid;
    int hgt;
    int x;
    int y;
    char **help_string;
}helps[] =
{
    { 24,   1,  25, 10, help_str0   },   // helps0
    { 72,   15, 3,  5,  help_str1   },   // helps1
    { 55,   1,  8,  13, help_str2   },   // helps2
    { 69,   5,  4,  8,  help_str3   },   // helps3
    { 66,   4,  4,  9,  help_str4   },   // helps4
    { 60,   4,  12, 10, help_str5   },   // helps5
    { 48,   2,  14, 12, help_str6   },   // helps6
    { 48,   2,  14, 12, help_str7   },   // helps7
    { 53,   1,  20, 13, help_str8   },   // helps8
    { 52,   1,  20, 13, help_str9   },   // helps9
    { 53,   1,  20, 13, help_str10  },   // helps10
    { 72,   5,   3, 10, help_str11  },   // helps11
    { 42,   7,  28,  8, help_str12  }    // helps12
};

//-----------------------------------------------------------------
//   Function extracts help num required and call it
//-----------------------------------------------------------------

BOOL    Help( MENU *M )
{
    DoHelp ( M -> entries[ M -> copt ].help );
    return ( TRUE );
}

//-------------------------------------------------------------------
// Provides a help screen for the command line
//-------------------------------------------------------------------

void CLHelp ( void )
{
    int     n;

    for (n = 0 ; n LT 8 ; n ++)    // misses the line that says
    {                               // press escape
        printf( " %s\n", help_str0[n] );
    }
}

//-----------------------------------------------------------------
//   Function prints help screens
//-----------------------------------------------------------------

void DoHelp( WORD num )
{
    WINDOW *help_win;
    int     n;              // loop variable
    char  **str = helps [ num ].help_string;

    help_win = WnMake( helps [ num ].wid, helps [ num ].hgt ,
                       helps [ num ].x, helps [ num ].y ,
                       HELP_FGC, HELP_BGC );
//    WnBorderCol ( help_win, V_LTWHITE, HELP_BGC );
//    WnBorderChg ( help_win, FALSE );   // dont change border when active

    for ( n = 0  ; n LT help_win -> hgt ; n ++, *str ++ )  // print the help message
        WnPrintxy ( help_win, 1, n, *str );

    WnOpen ( help_win );                // open the window
    VsDisp();                           // display it

    while (GetAKey() NE ESC )           // only exit if ESC pressed
        ;
    WnKill ( help_win );                // release window
}

BOOLEAN AboutDof ( void )
{
    extern int  in_dv;
    extern char registration_string[];
    int         x,
                y,
                fgc;

    WINDOW *about_win;

    about_win = WnMake( 60, 16, 9, 4, ABOUT_FGC, ABOUT_BGC );
    WnBorder ( about_win, DOUBLE );
/*
    WnSetAttr ( about_win, V_LTRED , V_BLACK );
    WnPrintxy ( about_win, LOGO_START, 1, "лллллллллл     ллллллл    лллллллллл");
    WnPrintxy ( about_win, LOGO_START, 2, "ллллллллллл   ллллллллл   лллллллллл");
    WnPrintxy ( about_win, LOGO_START, 3, "лллл   лллл  лллл   лллл  лллл"      );
    WnPrintxy ( about_win, LOGO_START, 4, "лллл    ллл  лллл   лллл  лллл"   );
    WnPrintxy ( about_win, LOGO_START, 5, "лллл    ллл  лллл   лллл  ллллллл"   );
    WnPrintxy ( about_win, LOGO_START, 6, "лллл    ллл  лллл   лллл  лллл"      );
    WnPrintxy ( about_win, LOGO_START, 7, "лллл   лллл  лллл   лллл  лллл"      );
    WnPrintxy ( about_win, LOGO_START, 8, "ллллллллллл   ллллллллл   лллл"      );
    WnPrintxy ( about_win, LOGO_START, 9, "лллллллллл     ллллллл    лллл"      );
*/
    WnSetAttr ( about_win, V_LTRED , V_BLACK );
    WnPrintxy ( about_win, LOGO_START, 1, "ЩЭЭЭЭЭЭЭЭЛ     ЩЭЭЭЭЭЛ    ЩЭЭЭЭЭЭЭЭЛ");
    WnPrintxy ( about_win, LOGO_START, 2, "К  ЩЭЭЛ  ШЛ   ЩМ ЩЭЛ ШЛ   К  ЩЭЭЭЭЭМ");
    WnPrintxy ( about_win, LOGO_START, 3, "К  К  ШЛ  К  ЩМ ЩМ ШЛ ШЛ  К  К"      );
    WnPrintxy ( about_win, LOGO_START, 4, "К  К   К  К  К  К   К  К  К  ШЭЭЛ"   );
    WnPrintxy ( about_win, LOGO_START, 5, "К  К   К  К  К  К   К  К  К  ЩЭЭМ"   );
    WnPrintxy ( about_win, LOGO_START, 6, "К  К   К  К  К  К   К  К  К  К"      );
    WnPrintxy ( about_win, LOGO_START, 7, "К  К  ЩМ  К  ШЛ ШЛ ЩМ ЩМ  К  К"      );
    WnPrintxy ( about_win, LOGO_START, 8, "К  ШЭЭМ  ЩМ   ШЛ ШЭМ ЩМ   К  К"      );
    WnPrintxy ( about_win, LOGO_START, 9, "ШЭЭЭЭЭЭЭЭМ     ШЭЭЭЭЭМ    ШЭЭМ"      );

    for ( y=1 ; y LT 10 ; y ++ )
        WnChgAttr ( about_win, LOGO_START + 13 , y , V_LTWHITE , V_BLACK , 11 );

    for ( y=1 ; y LT 10 ; y ++ )
        WnChgAttr ( about_win, LOGO_START + 26 , y , V_LTBLUE , V_BLACK , 10 );

    WnSetAttr ( about_win, V_LTCYAN , V_BLACK );
    WnPrintxy ( about_win, 2, 11, title_string[0] );

    WnSetAttr ( about_win, V_LTGREEN , V_BLACK );
    WnPrintxy ( about_win, 2, 12, title_string[1] );

    WnSetAttr ( about_win, V_LTMAGENTA , V_BLACK );
    WnPrintxy ( about_win, 15, 14, registration_string );

   if ( in_dv )     // double check for beta test version
   {
        WnSetAttr ( about_win, V_WHITE , V_BLACK );
        WnPrintxy ( about_win, 12, 15, "Currently running under DESQview" );
    }
     
    // change border colours of window


    WnOpen ( about_win );
    VsDisp();

    if ( GetAKey() EQ 'D' )
    {
        if ( GetAKey() EQ 'O' )
        {
            if ( GetAKey() EQ 'F' )
            {
                if ( GetAKey() EQ '2' )
                {
                    fgc = 0x00;
                    while ( NOT KeyPressed() )
                    {
                        fgc = ( fgc LT 0x0F ) ? fgc + 1 : 0x01;
                        for ( y=1 ; y LT 10 ; y ++ )
                            WnChgAttr ( about_win, LOGO_START , y ,
                                         fgc , BLACK , 11 );

                        TPause ( PAUSE_TICK );
                        VsDisp();

                        fgc = ( fgc LT 0x0F ) ? fgc + 1 : 0x01;
                        for ( y=1 ; y LT 10 ; y ++ )
                            WnChgAttr ( about_win, LOGO_START + 13 , y ,
                                        fgc , BLACK , 11 );

                        TPause ( PAUSE_TICK );
                        VsDisp();

                        fgc = ( fgc LT 0x0F ) ? fgc + 1 : 0x01;
                        for ( y=1 ; y LT 10 ; y ++ )
                            WnChgAttr ( about_win, LOGO_START + 26 , y ,
                                        fgc , BLACK , 10 );

                        TPause ( PAUSE_TICK );
                        VsDisp();
                    }
                    GetAKey();  // throw away the key
                }
            }
        }
    }
    WnKill ( about_win );   // release window
    return ( TRUE );
}
