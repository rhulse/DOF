//
//                  MENUFUNC.C
//     This module contains all the functions called from the menus.
//
//
#define NO_EXT_KEYS             // extensions disabled
#include <string.h>
#include <easyc.h>              // defines EQ LT GT etc
#include "window.h"
#include "screen.h"
#include "keyboard.h"
#include "dof.h"                // defines things
#include "menu.h"
#include "dir.h"
#include "globals.h"

//      Functions called by menu_main

BOOL    Option0( void );        // single entry option
BOOL    Option1( void );        // make a DOF chart

//      Functions called by all `static' menus

void    MainSetNewCopt ( MENU * , int );
void    SetNewCopt ( MENU * , int );
BOOLEAN CallPullDownCopt  ( MENU *);
BOOLEAN CallCurrentOption ( MENU *);
BOOLEAN CallFileMenuCopt ( MENU *);
BOOLEAN HomeKey ( MENU * );     // processes the home arrow key
BOOLEAN EndKey ( MENU * );      // processes the end arrow key
BOOLEAN DnKey ( MENU * );       // processes the dn arrow key
BOOLEAN UpKey ( MENU * );       // processes the up arrow key
BOOLEAN LeftKey ( MENU * );       // processes the left arrow key
BOOLEAN RightKey ( MENU * );       // processes the right arrow key
void    HiliteOpt ( MENU * );
void    HiliteOpt ( MENU * );
void    UnhiliteOpt ( MENU * );
void    MainHiliteOpt ( MENU *M );
void    MainUnhiliteOpt ( MENU *M );
BOOLEAN ReturnTrue ( void );
BOOLEAN ReturnFalse ( void );

extern WINDOW *input_win;

//  These functions set the current option and call the correct function

void    MainSetNewCopt ( MENU *M , int copt )
{
    MainUnhiliteOpt ( M );      // unhilite old option
    M -> copt = copt;           // change to new option
    MainHiliteOpt ( M );        // and hilite it
}

void    SetNewCopt ( MENU *M , int copt )
{
    UnhiliteOpt ( M );      // unhilite old option
    M -> copt = copt;       // change to new option
    HiliteOpt ( M );        // and hilite it
}

BOOLEAN CallPullDownCopt ( MENU *M )
{
    return ( ( M -> entries[ M -> copt ].function)( M ) );
}

BOOLEAN CallCurrentOption ( MENU *M )
{
    int     loop;
    extern  MENU MAIN_MENU;

    DimMenu ( M );          // dim calling menu
    DimPullDownMenu( &MAIN_MENU );  // and the main menu

    loop = ( M -> entries[ M -> copt ].function)( M );

    RestorePullDownMenu ( &MAIN_MENU ); // restore main menu
    RestoreMenu ( M );          // restore calling menu

    return ( loop );
}

BOOLEAN CallFileMenuCopt ( MENU *M )
{
    extern  MENU MAIN_MENU;
    WINDOW *no_files;
    char    file_name[13];      // buffer for the file name
    int     file_count,         // number of dof files
            temp = TRUE;       // value to return to caller

//      last entry of file menu is always QUIT
//      this checks to see if thats what we do !!!

    if ( M -> entries[ M -> copt].function EQ  ReturnFalse )
    {
        return ( ALT_Q );   // special key forces a quit
    }

    DimMenu ( M );          // dim calling menu
    DimPullDownMenu( &MAIN_MENU );  // and the main menu

    file_count = GetDofDirectory();

    if ( file_count )   // if there are any file then do this
    {
        if ( SelectDofFileName( file_name , M -> entries[ M -> copt].str ) )
        {
            temp = ( M -> entries[ M -> copt].function)( file_name );
        }
    }
    else    // Display a message
    {
        no_files = WnMake ( 29, 1, 31, 5, WARN_FGC, WARN_BGC );
        WnBorder ( no_files, DOUBLE );
//        WnBorderChg ( no_files, FALSE );
        WnPrintxy ( no_files, 1, 0, "No DOF files are available." );
        WnOpen( no_files );
        VsDisp();
        Bleep();
        GetAKey();
        WnKill ( no_files );                // release window
    }
    RestorePullDownMenu ( &MAIN_MENU ); // restore main menu
    RestoreMenu ( M );          // restore calling menu

    return ( temp );
}

//      Functions called by the main menu

//----------------------------------------------------------------
//  Single entry option. copt = 0
//----------------------------------------------------------------
BOOLEAN Option0( void )
{
    if ( PromptForValues( TRUE ) NE -1 )  // Asks for all parameters
    {
        DofCalc();                  // does these only if Prompt not aborted
        PrintSingle();
    }
    else
        WnKill ( input_win );       //  else kill window

    return ( TRUE );
}

//---------------------------------------------------------------
//  Create a DOF chart option. copt = 1
//---------------------------------------------------------------
BOOLEAN Option1( void )
{
    WINDOW *wait_win;

    if ( PromptForValues( FALSE ) NE -1 ) // Asks for Focal length only
    {
        WnKill ( input_win );
        wait_win = Message ( "Creating DOF file...please wait.", MESSAGE_FGC, MESSAGE_BGC );
        VsDisp();

        ChartDofs();
        WnKill ( wait_win );
    }
    else
        WnKill ( input_win ); // else kill window

    return ( TRUE );
}

//  functions called by all `static' menus

//-------------------------------------------------------------------
//   Function processes the pressing of the home key
//-------------------------------------------------------------------
BOOLEAN HomeKey ( MENU *M )
{
    SetNewCopt ( M, 0 );
    return ( TRUE );
}

//-------------------------------------------------------------------
//   Function processes the pressing of the end key.
//-------------------------------------------------------------------
BOOLEAN EndKey ( MENU *M )
{
    SetNewCopt ( M, M -> hgt - 1 );
    return ( TRUE );
}
//-------------------------------------------------------------------
//   Function processes the pressing of the down key.
//-------------------------------------------------------------------
BOOLEAN DnKey ( MENU *M )
{
    int copt = M -> copt;

    //  if copt LT etc then copt++ else copt = 0
    copt = ( copt LT M -> entry_count - 1 ) ? copt + 1 : 0;

    if ( M -> entries[ copt ].hot_key EQ NULL )
        copt ++;
    SetNewCopt ( M, copt );

    return ( TRUE );
}

//-------------------------------------------------------------------
//   Function processes the pressing of the up key.
//-------------------------------------------------------------------
BOOLEAN UpKey ( MENU *M )
{
    int copt = M -> copt;

    //  if copt GT 0 then copt-- else copt = last copt
    copt = ( copt GT 0 ) ? copt - 1 : M -> entry_count - 1;

    if ( M -> entries[ copt ].hot_key EQ NULL )
        copt --;

    SetNewCopt ( M, copt );

    return ( TRUE );
}

//-------------------------------------------------------------------
//   Function processes the pressing of the left key.
//-------------------------------------------------------------------
BOOLEAN LeftKey ( MENU *M )
{
    int copt = M -> copt;

    //  if copt GT 0 then copt-- else copt = last copt
    copt = ( copt GT 0 ) ? copt - 1 : M -> entry_count - 1;

    MainSetNewCopt ( M, copt );

    return ( TRUE );
}

//-------------------------------------------------------------------
//   Function processes the pressing of the left key.
//-------------------------------------------------------------------
BOOLEAN RightKey ( MENU *M )
{
    int copt = M -> copt;

    //  if copt LT etc then copt++ else copt = 0
    copt = ( copt LT M -> entry_count - 1 ) ? copt + 1 : 0;

    MainSetNewCopt ( M, copt );

    return ( TRUE );
}

//-------------------------------------------------------------------
//   Function hilites an option.
//-------------------------------------------------------------------
void HiliteOpt ( MENU *M )
{
    WnChgAttr ( M -> win_ptr, 0, M -> copt, BAR_FGC, BAR_BGC, M -> wid );
}

//-------------------------------------------------------------------
//   Function unhilites an option and restores its hilit letter.
//-------------------------------------------------------------------
void UnhiliteOpt ( MENU *M )
{
    WnChgAttr ( M -> win_ptr, 0, M -> copt, M -> fgc, M -> bgc, M -> wid );
    WnChgAttr ( M -> win_ptr, M -> entries[M -> copt].hil, M -> copt, HILITE_FGC, WIN_BGC, 1 );
}

//-------------------------------------------------------------------
//   Function hilites an option.
//-------------------------------------------------------------------
void MainHiliteOpt ( MENU *M )
{
    int start = M -> entries[ M -> copt ].hil;
    int    count = strlen ( M -> entries[ M -> copt ].str );

    WnChgAttr ( M -> win_ptr, start, 0, BAR_FGC, BAR_BGC, count );
}

//-------------------------------------------------------------------
//   Function unhilites an option and restores its hilit letter.
//-------------------------------------------------------------------
void  MainUnhiliteOpt ( MENU *M )
{
    int start = M -> entries[ M -> copt ].hil;
    int    count = strlen ( M -> entries[ M -> copt ].str );

    WnChgAttr ( M -> win_ptr, start, 0, M -> fgc, M -> bgc, count );
    WnChgAttr ( M -> win_ptr, start, 0, HILITE_FGC, WIN_BGC, 1 );
}


BOOLEAN ReturnTrue ( void )
{
    return ( TRUE );
}

BOOLEAN ReturnFalse ( void )
{
    return ( FALSE );
}
