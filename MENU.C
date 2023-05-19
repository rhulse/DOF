//
//                      MENU.C
//      This module contains all the bits to run the menus
//
//
#define NO_EXT_KEYS             // extensions disabled
#include <bios.h>
#include <ctype.h>
#include <easyc.h>              // defines EQ LT GT etc
#include "screen.h"
#include "window.h"
#include "keyboard.h"
#include "dof.h"                // defines things
#include "menu.h"
#include "help.h"
#include "view.h"
#include "file.h"
#include "menufunc.h"
#include "printer.h"
#include "status.h"
#include "globals.h"

int     RunPullDownMenu( void );      // run the menu module
int     RunMenu( MENU * );      // run the menu module
int     AppleMenu( void );      // `apple' menu .. uses menu module
int     FilesMenu( void );      // file menu .. uses menu module
int     CalcMenu( void );       // calculations menu
int     DefaultMenu( void );    // default menu .. uses menu module
int     FStopMenu( void );      // fstop menu .. uses menu module
int     DistMenu( void );       // dist menu .. uses menu module
MENU   *MakeAMenu( MENU * );    // makes a menu from a structure
MENU   *MakeMainMenu( MENU * );    // makes a menu from a structure
void    DimMenu( MENU * );      // dims the hilite letters on menus
void    DimPullDownMenu( MENU * ); // dims the hilite letters on pull down menu
void    RestoreMenu( MENU * );  // restores the hilite letters on menus
void    RestorePullDownMenu( MENU * );  // restores the hilite letters on menus

//
//  This is the genral structure of keys used by most of the menus
//
valid_key gen_menu_keys[] =
{
    {   ENTER  , CallCurrentOption },
    {   ESC    , ReturnFalse },
    {   L_ARROW, ReturnFalse },
    {   R_ARROW, ReturnFalse },
    {   U_ARROW, UpKey       },
    {   D_ARROW, DnKey       },
    {   HOME   , HomeKey     },
    {   END    , EndKey      },
    {   F1     , Help        },
};
#define num_gen_menu_keys  sizeof ( gen_menu_keys ) / sizeof ( valid_key )

//
//  These are the MENU structures !!
//

// MAIN MENU   looks like this ....
//     File   Calculations   Preferences

menu_entry main_menu_entries[] =
{
    {   "",            2,  1, ' ', AppleMenu   },
    {   "File",         6,  1, 'F', FilesMenu   },
    {   "Calculations", 13, 1, 'C', CalcMenu    },
    {   "Preferences",  28, 1, 'P', DefaultMenu }
};
#define num_main_menu_entries  sizeof (main_menu_entries) / sizeof (menu_entry)

valid_key main_menu_keys[] =
{
    {   D_ARROW, CallPullDownCopt },
    {   ENTER  , CallPullDownCopt },
    {   L_ARROW, LeftKey     },
    {   R_ARROW, RightKey    },
    {   F1     , Help        }
};
#define num_main_menu_keys  sizeof ( main_menu_keys ) / sizeof ( valid_key )

MENU MAIN_MENU =
{
    71, 1 ,
    0, 0, V_CYAN, V_BLACK ,
    0 ,
    " " ,
    0 ,
    main_menu_entries ,
    num_main_menu_entries,
    main_menu_keys ,
    num_main_menu_keys,
    (WINDOW * ) NULL
};

//  APPLE MENU

menu_entry apple_menu_entries[] =
{
    { "  About Dof"   , 2, 0,  'A', AboutDof    }
//    { " About System" , 7, 0,  'S', ReturnFalse }
};
#define num_apple_menu_entries  sizeof (apple_menu_entries) / sizeof (menu_entry)


MENU APPLE_MENU =
{
    14, num_apple_menu_entries ,
    2, 2, V_CYAN, V_BLACK ,
    0 ,
    " " ,
    0 ,
    apple_menu_entries ,
    num_apple_menu_entries ,
    gen_menu_keys ,
    num_gen_menu_keys ,
    (WINDOW * ) NULL
};

//  FILE MENU

menu_entry file_menu_entries[] =
{
    { " View" , 4, 0,  'V', ViewFile    },
    { " Copy" , 4, 0,  'C', CopyFile    },
    { " Move" , 4, 0,  'M', MoveFile    },
    { "Delete", 3, 0,  'D', DeleteFile  },
    { "Print" , 3, 0,  'P', PrintFile   },
    { "Ä"     , 0, 0,  NULL, NULL       },
    { " Quit" , 4, 2,  'Q', ReturnFalse }
};
#define num_file_menu_entries  sizeof (file_menu_entries) / sizeof (menu_entry)

valid_key file_menu_keys[] =        // different function for enter key
{
    {   ENTER  , CallFileMenuCopt },
    {   ESC    , ReturnFalse },
    {   L_ARROW, ReturnFalse },
    {   R_ARROW, ReturnFalse },
    {   U_ARROW, UpKey       },
    {   D_ARROW, DnKey       },
    {   HOME   , HomeKey     },
    {   END    , EndKey      },
    {   F1     , Help        },
};
#define num_file_menu_keys  sizeof ( file_menu_keys ) / sizeof ( valid_key )

MENU FILE_MENU =
{
    14, num_file_menu_entries ,
    6, 2, V_CYAN, V_BLACK ,
    0 ,
    " " ,
    3 ,
    file_menu_entries ,
    num_file_menu_entries ,
    file_menu_keys ,
    num_file_menu_keys ,
    (WINDOW * ) NULL
};

//  CALCULATIONS MENU
menu_entry calc_menu_entries[] =
{
    {   "Single Entries", 2, 3, 'S', Option0     },
    {   "  DOF Chart  ",  4, 4, 'D', Option1     }
};
#define num_calc_menu_entries  sizeof (calc_menu_entries) / sizeof (menu_entry)

MENU CALC_MENU =
{
    18, num_calc_menu_entries ,
    13, 2, V_CYAN, V_BLACK ,
    0 ,
    " " ,
    2 ,
    calc_menu_entries ,
    num_calc_menu_entries ,
    gen_menu_keys ,
    num_gen_menu_keys ,
    ( WINDOW * ) NULL
};

//  DEFAULT MENU

menu_entry pref_menu_entries[] =
{
    { "Circle of Confusion ", 1, 5, 'C', CircConf    },
    { "    F Stop Lists   ", 5, 6, 'F', FStopMenu   },
    { "  Distances Lists  ", 3, 7, 'D', DistMenu    }
};
#define num_pref_menu_entries  sizeof (pref_menu_entries) / sizeof (menu_entry)

MENU PREF_MENU =
{
    21, num_pref_menu_entries ,
    28, 2, V_CYAN, V_BLACK ,
    0 ,
    " " ,
    1 ,
    pref_menu_entries ,
    num_pref_menu_entries ,
    gen_menu_keys ,
    num_gen_menu_keys ,
    (WINDOW * ) NULL
};

//  FSTOP MENU

menu_entry fstop_menu_entries[] =
{
    { "Whole F Stops", 2, 0,  'W', UpdateFstopList },
    { "Half F Stops" , 2, 0,  'H', UpdateFstopList },
    { "Third F Stops", 2, 0,  'T', UpdateFstopList }
};
#define num_fstop_menu_entries  sizeof (fstop_menu_entries) / sizeof (menu_entry)

MENU FSTOP_MENU =
{
    18, num_fstop_menu_entries ,
    40, 4, V_CYAN, V_BLACK ,
    0 ,
    " ",
    2 ,
    fstop_menu_entries ,
    num_fstop_menu_entries ,
    gen_menu_keys ,
    num_gen_menu_keys ,
    ( WINDOW * ) NULL
};

//      DISTANCES MENU

menu_entry dist_menu_entries[] =
{
    {   "General List", 2, 8,  'G', UpdateDistList },
    {   " Macro List" , 3, 9,  'M', UpdateDistList },
    {   " Tele List"  , 3, 10, 'T', UpdateDistList }
};
#define num_dist_menu_entries  sizeof (dist_menu_entries) / sizeof (menu_entry)

MENU DIST_MENU =
{
    18, num_dist_menu_entries ,
    40, 5, V_CYAN, V_BLACK ,
    0,
    " " ,
    2 ,
    dist_menu_entries ,
    num_dist_menu_entries ,
    gen_menu_keys ,
    num_gen_menu_keys ,
    ( WINDOW * ) NULL
};

extern  WINDOW  *files_win;

//=---------------------------------
//      Runs all the menus
//=---------------------------------
int RunPullDownMenu( void )
{
    MENU   *M;
    int     n;
    WORD    entry,
            loop = TRUE;

    M = MakeMainMenu ( &MAIN_MENU );    // make the menu and return pointer
    WnOpen ( M -> win_ptr );       // open the menus window

    do                             // Main loop starts here
    {
        MainHiliteOpt ( M );       // hilite the current option
        VsDisp();                  // update the display

         // Wait for a key to be pressed

        if ( ( entry = GetAKey() ) EQ ALT_Q )       // q. do we exit
        {                                           // a. yes
            loop = FALSE;   break;                  // break out and exit !!!!
        }
        else if ( isalpha ( entry BAND 0x00FF ) )   // q. is it alpha
        {                                           // a. process hot keys
            entry = toupper ( entry );
            n = M -> entry_count;

            while ( n-- )
            {
                if ( entry EQ M -> entries[ n ].hot_key )
                {
                    MainSetNewCopt ( M, n );    // set  new copt ...
                    entry = ENTER; break;   // fake an ENTER and break out
                }
            }
        }
                             // process non hot keys
        do
        {
            n = M -> key_count;
            while ( n-- )
            {   // get and run the appropriate function then break out of loop
                if ( entry EQ M -> keys[ n ].key )
                {
                    loop = ( M -> keys[ n ].function)( M );
                    if ( loop EQ L_ARROW )
                    {    // fake enter key to get to the next menu
                        LeftKey( M ); entry = ENTER; MainHiliteOpt ( M );
                    }
                    else if ( loop EQ R_ARROW )
                    {    // fake enter key to get to the next menu
                        RightKey( M ); entry = ENTER; MainHiliteOpt ( M );
                    }
                    else if ( loop EQ ALT_Q )
                    {
                        loop = FALSE;   // time to exit the programme !!!!!
                    }
                    break;
                }
            }
        }while ( ( loop EQ L_ARROW ) OR ( loop EQ R_ARROW ) );
    }
    while( loop );

    WnKill ( M -> win_ptr );
    M -> win_ptr = NULL;

    return( entry );
}

//=---------------------------------
//      Runs all the menus
//=---------------------------------
int RunMenu( MENU *M )
{
    int     n;
    WORD    entry,
            loop = TRUE;

    MakeAMenu( M );              // make the menu
    WnOpen ( M -> win_ptr );        // open the menus window

    do                              // Main loop starts here
    {
        HiliteOpt( M );       // hilite the current option
        VsDisp();                   // update the display

         // Wait for a key to be pressed

        if ( ( entry = GetAKey() ) EQ ALT_Q )
        {
            loop = FALSE; break;    // break out and exit DOF
        }
        else if ( isalpha ( entry BAND 0x00FF ) )    // process hot keys
        {
            entry = toupper ( entry );
            n = M -> entry_count;

            while ( n-- )
            {
                if ( entry EQ M -> entries[ n ].hot_key )
                {
                    SetNewCopt( M, n );    // set new copt ...
                    entry = ENTER; break;   // fake an ENTER and break out
                }
            }
        }
        // process non hot keys

        n = M -> key_count;
        while ( n-- )
        {
            if ( entry EQ M -> keys[ n ].key )
            {   // get and run the appropriate function
                loop = ( M -> keys[ n ].function)( M );
                if ( loop EQ ALT_Q )        // capture call to QUIT
                {
                    loop = FALSE; entry = ALT_Q;    // special exit !!
                }
                break;  // key found so break out of loop
            }
        }
    }
    while( loop );

    WnKill ( M -> win_ptr );
    M -> win_ptr = NULL;

    return( entry );
}

//-------------------------------------------------------------------
//   Function runs the `apple' menu.
//-------------------------------------------------------------------

int     AppleMenu( void )
{
    return ( RunMenu ( &APPLE_MENU ) );
}

//-------------------------------------------------------------------
//   Function runs the files menu with help and sets WINDOW copt.
//-------------------------------------------------------------------

int     FilesMenu( void )
{
    return ( RunMenu ( &FILE_MENU ) );
}

//-------------------------------------------------------------------
//   Function runs the calculations menu with help and sets WINDOW copt.
//-------------------------------------------------------------------

int     CalcMenu( void )
{
    return ( RunMenu ( &CALC_MENU ) );
}

//-------------------------------------------------------------------
//  Function runs the default menu with help and sets WINDOW copt.
//-------------------------------------------------------------------

int     DefaultMenu( void )
{
    return ( RunMenu( &PREF_MENU ) );
}

//-------------------------------------------------------------------
//   Function runs the fstop menu with help and sets WINDOW copt.
//-------------------------------------------------------------------

int     FStopMenu( void )
{
    int temp = FALSE;
    extern  int fstop_list;

    FSTOP_MENU.copt = fstop_list;

    if ( ( temp = RunMenu( &FSTOP_MENU ) ) NE ESC )
    {                       // if it was not escape then update
        PrintFSList();
    }
    return ( temp );
}

//-------------------------------------------------------------------
//   Function runs the dist menu with help and sets WINDOW copt.
//-------------------------------------------------------------------

int     DistMenu( void )
{
    int temp = FALSE;
    extern  int distances_list;

    DIST_MENU.copt = distances_list;

    if ( ( temp = RunMenu( &DIST_MENU ) ) NE ESC )  // only update if not ESC
    {
        PrintDList();
    }
    return ( temp );
}

//-------------------------------------------------------------------
//   Function makes a menu and puts text in it.
//-------------------------------------------------------------------

MENU *MakeAMenu( MENU *M )
{
    WINDOW *ptr;
    int n, z, hgt = M -> hgt;

    ptr = WnMake( M -> wid, M -> hgt, M -> x, M -> y, WIN_FGC, WIN_BGC );

    for ( n = 0 ; n LT hgt ; n++ )
    {
        if ( M -> entries[n].hot_key EQ NULL )
        {
            z = M -> wid;
            while ( z-- )
            {
                WnPrintxy ( ptr, z, n, M -> entries[n].str );
            }
        }
        else
        {
            WnPrintxy ( ptr, M -> offset, n, M -> entries[n].str );
            WnChgAttr ( ptr, M -> entries[n].hil, n, HILITE_FGC, WIN_BGC, 1 );
        }
    }
    M -> win_ptr = ptr;

    return ( M );
}

MENU *MakeMainMenu( MENU *M )
{
    WINDOW *ptr;
    int     n,
            hgt = M -> hgt;

    ptr = WnMake( M -> wid, M -> hgt, M -> x, M -> y, M -> fgc, M -> bgc );
    WnBorder ( ptr, FALSE );

    for ( n = 0 ; n LT num_main_menu_entries ; n++ )
    {
            WnPrintxy ( ptr, M -> entries [n].hil, 0, M -> entries[n].str );
            WnChgAttr ( ptr, M -> entries[n].hil, 0, HILITE_FGC, WIN_BGC, 1 );
    }
    M -> win_ptr = ptr;

    return ( M );
}

void    DimMenu( MENU *M )
{
    int n;

    UnhiliteOpt ( M );       // unhilte the current option

    for ( n = 0 ; n LT M -> entry_count ; n++ )
        WnChgAttr ( M -> win_ptr, M -> entries[n].hil, n, WIN_FGC, WIN_BGC, 1 );

}

void    DimPullDownMenu( MENU *M )
{
    int     n;

    MainUnhiliteOpt ( M );       // unhilte the current option

    for ( n = 0 ; n LT M -> entry_count ; n++ )
        WnChgAttr ( M -> win_ptr, M -> entries[n].hil, 0, WIN_FGC, WIN_BGC, 1 );

}

void    RestoreMenu( MENU *M )
{
    int     n;

//  restore all hilites

    for ( n = 0 ; n LT M -> entry_count ; n++ )
        if ( M -> entries[n].hot_key NE NULL )   // miss spacer lines
            WnChgAttr ( M -> win_ptr, M -> entries[n].hil, n,
                        HILITE_FGC, WIN_BGC, 1 );
}

void    RestorePullDownMenu( MENU *M )
{
    int     n;

//  restore all hilites

    for ( n = 0 ; n LT M -> entry_count ; n++ )
        WnChgAttr ( M -> win_ptr, M -> entries[n].hil, 0, HILITE_FGC, WIN_BGC, 1 );

    MainHiliteOpt ( M );

}
