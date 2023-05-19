//
//                          VIEW.C
//      This module contains the view function of the file menu.
//
//
#define NO_EXT_KEYS     // extensions disabled
#include <memory.h>
#include <malloc.h>
#include <stdio.h>
#include <easyc.h>      // defines EQ LT GT etc
#include "mylib.h"
#include "window.h"
#include "screen.h"
#include "keyboard.h"
#include "dof.h"
#include "globals.h"
#include "menu.h"
#include "file.h"
#include "help.h"
#include "menufunc.h"
#include "printer.h"

BOOL    ViewFile( char * );
void    DoView( WINDOW * , char * );
void    ViewPrint( WINDOW * );
WINDOW *MakeViewWin( void );
int     LoadFile ( char * );
BOOL    ViewHomeKey ( WINDOW * );
BOOL    ViewEndKey ( WINDOW * );
BOOL    ViewPgDnKey ( WINDOW * );
BOOL    ViewPgUpKey ( WINDOW * );
BOOL    ViewDnKey ( WINDOW * );
BOOL    ViewUpKey ( WINDOW * );
void    VDoTopLine ( WINDOW * );
void    VDoBottomLine ( WINDOW * );
BOOL    VMoveLeft ( WINDOW * );
BOOL    VMoveRight ( WINDOW * );
void    VHilite ( WINDOW * );
void    VUNHilite ( WINDOW * );

valid_key view_keys[] =
{
    {   ESC     , ReturnFalse   },
    {   F1      , ReturnTrue    },
    {   F2      , ReturnTrue    },
    {   L_ARROW , VMoveLeft     },
    {   R_ARROW , VMoveRight    },
    {   U_ARROW , ViewUpKey     },
    {   D_ARROW , ViewDnKey     },
    {   PG_UP   , ViewPgUpKey   },
    {   PG_DN   , ViewPgDnKey   },
    {   HOME    , ViewHomeKey   },
    {   END     , ViewEndKey    }
};
#define num_view_keys  sizeof ( view_keys ) / sizeof ( valid_key )

valid_key alt_keys[] =
{
    {   ALT_C   , CopyFile      },
    {   ALT_M   , MoveFile      },
    {   ALT_D   , DeleteFile    },
    {   ALT_P   , PrintFile     }
};
#define num_alt_keys  sizeof ( alt_keys ) / sizeof ( valid_key )

struct line
{
    char    text[85];
    struct  line  *prev;    //   ptr to previous line
    struct  line  *next;    //   ptr to next line
} *line_start, *line_end, *view_top_line, *view_bottom_line;

   //------------------------------------------------------------------
   //       Function sets up bits for DoView
   //------------------------------------------------------------------

BOOL ViewFile( char *file )
{
    WINDOW *view_win;

    if ( LoadFile ( file ) )
    {
        view_win = MakeViewWin();
        ViewPrint( view_win );  // print the file in the window
        WnOpen ( view_win );    // open it

        DoView ( view_win , file );    // view the file
    }
    else
    {
        view_win = WnMake ( 40, 2, 18, 8, WARN_FGC, WARN_BGC );
        WnBorder ( view_win, DOUBLE );

        WnPrintxy ( view_win, 7, 0, "Error reading the file." );
        WnPrintxy ( view_win, 7, 1, "Press a key to continue." );
        WnOpen ( view_win );

        VsDisp();
        while (GetAKey() NE ESC )
            ;
    }
    WnKill ( view_win );    // free it when finished
    return ( TRUE );
}

   //------------------------------------------------------------------
   //       Function views a DOF file.
   //------------------------------------------------------------------

void DoView( WINDOW *win , char *file_name )
{
    WORD            entry;
    int             n,
                    loop = TRUE;
    struct line    *temp;

    do        // Main loop starts here
    {
        VsDisp();

        // Wait for a key to be pressed and switch on it.

        if ( ( entry = GetAKey() ) EQ F1 )  // help !!
            DoHelp ( VIEW_HELP );
        else
        {
            n = num_view_keys;  // check for movement keys
            while ( n-- )
            {
                if ( entry EQ view_keys[n].key )
                    loop = ( view_keys[n].function)( win );
            }

            n = num_alt_keys;  // check for alternate keys for action to take
            while ( n-- )
            {
                if ( entry EQ alt_keys[n].key )
                    loop = ( alt_keys[n].function)( file_name );
            }

            // did the user try to and was the file deleted
            if ( ( entry EQ ALT_D ) AND ( file_name[0] EQ '\0' ) )
                loop = FALSE;           // yes - then exit
        }
    }
    while( loop );

    while ( line_start ) // free any file data
    {
        temp = line_start;
        line_start = line_start -> next;
        free ( temp );
    }
}

   //------------------------------------------------------------------
   //       Prints the file in the file view window.
   //------------------------------------------------------------------

void ViewPrint( WINDOW *win )
{
    int     n,
            hgt = win -> hgt ;

    view_top_line = line_start;
    view_bottom_line = line_start;

    for ( n = 0 ; n LT hgt ; n++ )
    {
        WnPrintxy ( win, 0, n, view_bottom_line -> text );
        WnChgAttr ( win, win -> copt * 7, n, BAR_FGC, BAR_BGC, 7 );
            if ( n NE hgt - 1 )
                view_bottom_line = view_bottom_line -> next;
    }
}

   //------------------------------------------------------------------
   //       Make the viewing window.
   //------------------------------------------------------------------

WINDOW *MakeViewWin( void )
{
    WINDOW *temp;

    static char *view_win_title =
    "[  File Viewing Window   ]อออออออ[ Press F1 for help or <Esc> to exit. ]";

    temp = WnMake ( 78, 22, 1, 2, WIN_FGC, WIN_BGC );
    WnBorder ( temp, DOUBLE );
    WnBorderCol ( temp, HILITE_FGC, WIN_BGC ); // make it look active

    WnTitle ( temp, view_win_title );  // set the title

    temp -> copt = 4;   // vertical highlight set in the middle of the screen

    return ( temp );
}

   //-------------------------------------------------------------------
   //   Load the file into memory for viewing.
   //-------------------------------------------------------------------

int LoadFile ( char *name )
{
    WINDOW         *message;
    struct line    *info,
                   *new;
    char            buffer[81];
    FILE            *fp;

    if ( ( fp = fopen ( name, "rt" ) ) EQ NULL )
    {
        return ( FALSE );
    }

    message = Message( "Loading File....", MESSAGE_FGC, MESSAGE_BGC );
    VsDisp();

    if ( ( line_start = ( struct line * )malloc ( sizeof ( struct line ) ) ) EQ NULL )
    {
        AllocError();
    }
    info = line_start;
    info -> prev = NULL;    // first entry points back to a NULL
    info -> next = NULL;
    info -> text[0] = NULL;

    if ( ( fgets ( buffer, 81, fp ) ) ) // read the first line - throw away
    {
        fgets ( buffer, 81, fp );       // read the second line - throw away

        fgets ( info -> text, 81, fp ); // read the third line - keep

        // find the new line char and replace with a NUL

        FindReplace( info -> text, '\n', '\0' );

        while ( ( fgets ( buffer, 81, fp ) ) )
        {
            // allocate space for the next line

            if ( NOT ( new = ( struct line * )malloc ( sizeof ( struct line ) ) ) )
                AllocError();

            FindReplace( buffer, '\n', '\0' );

            memcpy ( new -> text, buffer, 81 );

            info -> next = new;
            info -> next -> prev = info;       // add to linked list
            info = new;
        }
        info -> next = NULL;   // last entry points on to NULL
        line_end = info;
    }
    fclose ( fp );
    WnKill ( message );

    return ( TRUE );
}

   //-------------------------------------------------------------------
   //    Processes the HOME key
   //-------------------------------------------------------------------

BOOL ViewHomeKey ( WINDOW *win )
{
    while ( view_top_line -> prev NE NULL )
        VDoTopLine( win );
    return ( TRUE );
}

   //-------------------------------------------------------------------
   //    Processes the END key
   //-------------------------------------------------------------------

BOOL ViewEndKey ( WINDOW *win )
{
    while ( view_bottom_line -> next NE NULL )
        VDoBottomLine( win );
    return ( TRUE );
}

   //-------------------------------------------------------------------
   //   Processes the page down key
   //-------------------------------------------------------------------

BOOL ViewPgDnKey ( WINDOW *win )
{
    int     n = win -> hgt - 1;

    while ( ( view_bottom_line -> next NE NULL ) AND ( n-- NE 0 ) )
        VDoBottomLine( win );
    return ( TRUE );
}

   //-------------------------------------------------------------------
   //   Processes the page up key
   //-------------------------------------------------------------------

BOOL ViewPgUpKey ( WINDOW *win )
{
    int     n = win -> hgt - 1;

    while ( ( view_top_line -> prev NE NULL ) AND ( n-- NE 0 ) )
        VDoTopLine( win );
    return ( TRUE );
}
   //-------------------------------------------------------------------
   //   Processes the up arrow
   //-------------------------------------------------------------------

BOOL ViewDnKey ( WINDOW *win )
{
    if ( view_bottom_line -> next NE NULL )
        VDoBottomLine( win );
    return ( TRUE );
}

   //-------------------------------------------------------------------
   // Processes the down arrow
   //-------------------------------------------------------------------

BOOL ViewUpKey ( WINDOW *win )
{
    if ( view_top_line -> prev NE NULL )
        VDoTopLine( win );
    return ( TRUE );
}

   //-------------------------------------------------------------------
   // Erases the top line and replaces it.
   //-------------------------------------------------------------------

void VDoTopLine ( WINDOW *win )
{
    view_top_line = view_top_line -> prev;
    view_bottom_line = view_bottom_line -> prev;
    WnScroll ( win, DOWN, 0, win -> hgt - 1 );
    WnCls ( win, 0, 1 );
    WnPrintxy ( win, 0, 0, view_top_line -> text );
    WnChgAttr ( win, win -> copt * 7, 0, BAR_FGC, BAR_BGC, 7 );
}

   //-------------------------------------------------------------------
   // Erases the bottom line and replaces it.
   //-------------------------------------------------------------------

void VDoBottomLine ( WINDOW *win )
{
    view_bottom_line = view_bottom_line -> next;
    view_top_line = view_top_line -> next;
    WnScroll ( win, UP, 1, win -> hgt - 1 );
    WnCls ( win, win -> hgt - 1, 1 );
    WnPrintxy ( win, 0, win -> hgt - 1, view_bottom_line -> text );
    WnChgAttr ( win, win -> copt * 7, win -> hgt -1, BAR_FGC, BAR_BGC, 7 );
}

   //-------------------------------------------------------------------
   // Moves the vertical highlight bar to the left
   //-------------------------------------------------------------------

BOOL VMoveLeft ( WINDOW *win )
{
    int     copt = win -> copt;

    VUNHilite ( win );
    win -> copt = ( copt GT 0 ) ? copt - 1 : 10;
    VHilite ( win );
    return ( TRUE );
}

   //-------------------------------------------------------------------
   // Moves the vertical highlight bar to the right
   //-------------------------------------------------------------------

BOOL    VMoveRight ( WINDOW *win )
{
    int     copt = win -> copt;

    VUNHilite ( win );
    win -> copt = ( copt LT 10 ) ? copt + 1 : 0;
    VHilite ( win );
    return ( TRUE );
}

   //-------------------------------------------------------------------
   // Highites a column.
   //-------------------------------------------------------------------

void VHilite ( WINDOW *ptr )
{
    int     n,
            hgt = ptr -> hgt ;

    for  ( n = 0; n LT hgt; n++ )
        WnChgAttr ( ptr, ptr -> copt * 7, n, BAR_FGC, BAR_BGC, 7 );
}

   //-------------------------------------------------------------------
   // UNHighites a column.
   //-------------------------------------------------------------------

void VUNHilite ( WINDOW *ptr )
{
    int     n,
            hgt = ptr -> hgt;

    for  ( n = 0; n LT hgt; n++ )
        WnChgAttr ( ptr, ptr -> copt * 7, n , ptr -> fgc, ptr -> bgc, 7 );
}
