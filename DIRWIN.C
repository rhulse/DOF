//
//                          FILEWIN.C
//      This module contains the file window functions.
//
//
#define NO_EXT_KEYS // extensions disabled
#include <easyc.h>             // defines EQ LT GT etc
#include "window.h"
#include "screen.h"
#include "keyboard.h"
#include "dof.h"        // defines menu and help strings
#include "globals.h"
#include "dir.h"
#include "menu.h"
#include "help.h"
#include "menufunc.h"

BOOL    RunFileWin( WINDOW * );
BOOL    FileHomeKey ( WINDOW * );
BOOL    FileEndKey ( WINDOW * );
BOOL    FilePgUpKey ( WINDOW * );
BOOL    FilePgDnKey ( WINDOW * );
BOOL    FileUpKey ( WINDOW * );
BOOL    FileDnKey ( WINDOW * );
void    FHiliteOpt ( WINDOW * );
void    FUnhiliteOpt ( WINDOW * );
BOOL    DoTopLine ( WINDOW * );
BOOL    DoBottomLine ( WINDOW * );
BOOL    PrintDir( WINDOW * );

valid_key dir_keys[] =
{
    {   ESC    , ReturnFalse },
    {   F1     , ReturnTrue  },
    {   F2     , ReturnTrue  },
    {   U_ARROW, FileUpKey   },
    {   D_ARROW, FileDnKey   },
    {   PG_UP  , FilePgUpKey },
    {   PG_DN  , FilePgDnKey },
    {   HOME   , FileHomeKey },
    {   END    , FileEndKey  }
};
#define num_dir_keys  sizeof ( dir_keys ) / sizeof ( valid_key )

   //-------------------------------------------------------------------
   //  Runs the window with the file names in it.
   //-------------------------------------------------------------------

BOOL RunFileWin( WINDOW *win )
{
    int     entry,
            n,
            file_selected = FALSE,
            loop = TRUE;

    win -> copt = 0;            // top line and first file
    PrintDir( win );            // print the directory first time
    node = top_line;          // hilite is first line

    do        // Main loop starts here
    {
        PrintDir( win );        // print the directory
        FHiliteOpt ( win );     // hilite the current file
        VsDisp();               // display it

        entry = GetAKey();  // Wait for a key to be pressed and switch on it.
        if ( entry EQ ENTER )
        {
            loop = FALSE; file_selected = TRUE;
        }
        else if ( entry EQ F1 )
        {
            DoHelp( SELECT_HELP );
        }
        else
        {
            n = num_dir_keys;

            while ( n-- )
            {
                if ( entry EQ dir_keys[n].key )
                {
                    FUnhiliteOpt ( win );
                    loop = ( dir_keys[n].function)( win );
                    FHiliteOpt ( win );     // hilite the current file
                }
            }
        }
    }
    while( loop );

    return ( file_selected );
}

   //-------------------------------------------------------------------
   //    Processes the HOME key
   //-------------------------------------------------------------------

BOOL FileHomeKey ( WINDOW *win )
{
    while ( DoTopLine( win ) )

    node = top_line;
    win -> copt = 0;

    return ( TRUE );
}

   //-------------------------------------------------------------------
   //    Processes the END key
   //-------------------------------------------------------------------

BOOL FileEndKey ( WINDOW *win )
{
    while ( DoBottomLine( win ) )

    node = bottom_line;
    win -> copt = win -> hgt - 1;

    return ( TRUE );
}

   //-------------------------------------------------------------------
   //   Processes the page up key
   //-------------------------------------------------------------------

BOOL FilePgUpKey ( WINDOW *win )
{
    int n = win -> hgt - 1;

    while ( n-- )
        DoTopLine( win );

    return ( TRUE );
}

   //-------------------------------------------------------------------
   //   Processes the page down key
   //-------------------------------------------------------------------

BOOL FilePgDnKey ( WINDOW *win )
{
    int n = win -> hgt - 1;

    while ( n-- )
        DoBottomLine( win );

    return ( TRUE );
}

   //-------------------------------------------------------------------
   //   Processes the up arrow
   //-------------------------------------------------------------------

BOOL FileUpKey ( WINDOW *win )
{
    if ( win -> copt GT 0 )
    {
        win -> copt--;
        node = node -> prev;
    }
    else
        DoTopLine( win );

    return ( TRUE );
}

   //-------------------------------------------------------------------
   // Processes the down arrow
   //-------------------------------------------------------------------

BOOL FileDnKey ( WINDOW *win )
{
    if ( win -> copt LT win -> hgt -1 )
    {
        win -> copt++;
        node = node -> next;
    }
    else
        DoBottomLine( win );

    return  ( TRUE );
}

   //-------------------------------------------------------------------
   //   Function hilites an option.
   //-------------------------------------------------------------------

void FHiliteOpt ( WINDOW *win )
{
    WnChgAttr ( win, 0, win -> copt, BAR_FGC, BAR_BGC, win -> wid );
}

   //-------------------------------------------------------------------
   // Unhilites the current option.
   //-------------------------------------------------------------------

void FUnhiliteOpt ( WINDOW *win )
{
    WnChgAttr ( win, 0, win -> copt, win -> fgc, win -> bgc, win -> wid );
}

   //-------------------------------------------------------------------
   // Erases the top line and replaces it.
   //-------------------------------------------------------------------

BOOL DoTopLine ( WINDOW *win )
{
    int temp = FALSE;

    if ( top_line -> prev NE NULL )
    {
        top_line = top_line -> prev;
        bottom_line = bottom_line -> prev;
        node = node -> prev;
        temp++;
    }
    return ( temp );
}

   //-------------------------------------------------------------------
   // Erases the bottom line and replaces it.
   //-------------------------------------------------------------------

BOOL DoBottomLine ( WINDOW *win )
{
    int temp = FALSE;

    if ( bottom_line -> next NE NULL )
    {
        bottom_line = bottom_line -> next;
        top_line = top_line -> next;
        node = node -> next;
        temp++;
    }
    return ( temp );
}

   //-------------------------------------------------------------------
   //  Reprints the directory.
   //-------------------------------------------------------------------

BOOL PrintDir( WINDOW *win )
{
    int     n,
            hgt = win -> hgt ;

    bottom_line = top_line;

    for ( n = 0 ; n LT hgt ; n++ )
    {
        WnCls ( win, n, 1 );
        WnPrintxy ( win, 2, n, bottom_line -> file_name );
        if ( n NE hgt - 1 )
            bottom_line = bottom_line -> next;
    }
    return ( TRUE );
}

