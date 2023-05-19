//
//                    DIR.C
//      This module contains the functions to Get a sorted directory
//     and all the associated bits.
//
//
#define NO_EXT_KEYS // extensions disabled
#include <io.h>
#include <dos.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <easyc.h>             // defines EQ LT GT etc
#include "mylib.h"
#include "keyboard.h"
#include "window.h"
#include "screen.h"
#include "dirwin.h"
#include "dof.h"        // defines menu and help strings
#include "globals.h"

#define SELECT_FGC V_YELLOW
#define SELECT_BGC V_CYAN
#define SELECT_X  25
#define SELECT_Y  5

//structures for directory lists

struct directory *first , *node , *new , *top_line , *bottom_line;

BOOL    SelectDofFileName( char * , char * );
int     GetDofDirectory( void );
void    KillDirEntry ( struct directory * );

static int     file_count = 0;

BOOL    SelectDofFileName( char *file_name , char *action_name )
{
    WINDOW *select_win,
           *file_dir_win;
    int     hgt,
            sel_hgt,
            file_selected ;

    hgt = ( file_count GT 11 ) ? 11 : file_count;

    sel_hgt = ( hgt LE 5 ) ? 7 : hgt + 2;
//
//  the outside window
//
    select_win = WnMake ( 40, sel_hgt, SELECT_X, SELECT_Y, SELECT_FGC, SELECT_BGC );
    WnPrintxy ( select_win, 1, 0, action_name );
    WnPrintxy ( select_win, 7, 0, "....." );
    WnPrintxy ( select_win, 0, 2, " Use arrow keys\n and <enter> to\n select a file\n or <esc> to exit.");
    WnBorder ( select_win, DOUBLE );
    WnBorderCol ( select_win, HILITE_FGC, SELECT_BGC );
    WnOpen ( select_win );
//
//  the `view port' window for files
//
    file_dir_win = WnMake ( 15, hgt, SELECT_X + 20, SELECT_Y + 1, WIN_FGC, WIN_BGC );
    WnBorderCol ( file_dir_win, HILITE_FGC, SELECT_BGC );
//    WnBorderChg ( file_dir_win, FALSE );
    WnShadowOff ( file_dir_win );
    WnOpen ( file_dir_win );

    if ( ( file_selected = RunFileWin ( file_dir_win ) ) )
    {
        strcpy ( file_name, node -> file_name );
    }
    VsDisp();
    WnKill ( file_dir_win );
    WnKill ( select_win );

    while ( first NE NULL )     // releases all memory associated with directory
        KillDirEntry ( first );

    return ( file_selected );   // was a file selected ???
}
   //-------------------------------------------------------------------
   // Creates a doubly linked list with the directory entries in it.
   //-------------------------------------------------------------------

int GetDofDirectory( void )
{
    struct dta  file_info;

    file_count = 0;

    if ( NOT DosFindFirst ( "????????.DOF", 0, &file_info ) )
    {
        if ( ( first = malloc ( sizeof ( struct directory ) ) ) EQ NULL )
        {
            AllocError();
        }
        else
        {
            new = first;
            new -> prev = NULL;
            new -> next = NULL;
            strcpy ( new -> file_name, file_info.name );
            file_count++;
        }

        while ( NOT DosFindNext ( &file_info ) )
        {
            if ( ( new = malloc ( sizeof ( struct directory ) ) ) EQ NULL )
            {
                AllocError();
            }
            else
            {
                strcpy ( new -> file_name, file_info.name );
                file_count++;
                node = first;

                while ( node )
                {
                    if ( strcmp ( new -> file_name, node -> file_name ) LE 0 )
                    {
                        if ( node NE first )
                        {
                            node -> prev -> next = new;
                            new -> prev = node -> prev;
                            new -> next = node;
                            node -> prev = new;
                        }
                        else
                        {
                            new -> prev = NULL;
                            new -> next = first;
                            first -> prev = new;
                            first = new;
                        }
                        break;
                    }
                    else if ( node -> next EQ NULL )
                    {
                        node -> next = new;
                        new -> prev = node;
                        new -> next = NULL;
                        break;
                    }
                    else
                        node = node -> next;
                }
            }
        }
    }
    top_line = first;   // topline is first entry.
    node = top_line;    // node is topline.
    return ( file_count );
}


void KillDirEntry ( struct directory *ptr )
{
    struct directory   *on_ptr = ptr -> next,
                       *back_ptr = ptr -> prev;

    // this bit bypasses and frees the structure pointed to by  'ptr'

    if ( back_ptr EQ NULL )
        first = on_ptr;
    else
        back_ptr -> next = on_ptr;

    if ( on_ptr NE NULL )
        on_ptr -> prev = back_ptr;

    free ( ptr );
}
