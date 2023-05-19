#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <easyc.h>
#include "window.h"
#include "screen.h"
#include "keyboard.h"
#include "dof.h"
#include "globals.h"
#include "menu.h"

#define STATUS_FGC V_YELLOW
#define STATUS_BGC V_MAGENTA

void    CreateStatusLine( void );
void    CircConf( void );   // handles menu option
char   *CreateCC ( char * ); // creates asciiz of circle of conf
void    PrintCC( void );    // print circle of conf in title window
void    PrintDList( void );
void    PrintFSList( void );
BOOLEAN UpdateFstopList ( MENU * );
BOOLEAN UpdateDistList ( MENU * );

static WINDOW *status_line;     // global in this module only

int     fstop_list = 0;
int     distances_list = 0;

char *brackets = "[               ]";

extern  int     error_flag;
extern  char    spaces8[];
extern  char    spaces13[];

   //------------------------------------------------------------------
   //       Function creates a status line to write to
   //------------------------------------------------------------------

void CreateStatusLine( void )
{
	status_line = WnMake ( 80, 1, 0, 24, STATUS_FGC, STATUS_BGC );

	WnPrintxy ( status_line, 0, 0, "Circle of Confusion =" );
	PrintCC();
	PrintFSList();
	PrintDList();
	WnOpen ( status_line );
}

   //------------------------------------------------------------------
   //    Handles the circle of confusion default option.
   //------------------------------------------------------------------

void CircConf( void )
{
	char buffer[35];
	float temp;
	WINDOW *get_cc_win;

	memset ( buffer, ' ', 35 );   buffer[35] = '\0';

	get_cc_win = Message ( "Change to -> ", MESSAGE_FGC, MESSAGE_BGC );
	VsDisp();

	if ( DWnEdit ( get_cc_win, 35, 17, 0, buffer ) )
	{
		temp = CheckInput( buffer, (float)1.0, (float)1000 );
		if ( error_flag )
			Bleep();
		else
		{
			cir_conf = temp;
			PrintCC();
		}
	}
	WnKill ( get_cc_win );
}

   //------------------------------------------------------------------
   //    Create the circle of confusion figure for the status line.
   //------------------------------------------------------------------

char *CreateCC ( char *buffer )
{
	char    buffer2[5];
	float   temp;

	sprintf ( buffer , "%-1.0f %s    " ,
	( cir_conf LT 1000 ) ? cir_conf : cir_conf/1000 ,
	( cir_conf LT 1000 ) ? "æm" : "mm" );

	return ( buffer );

}

   //------------------------------------------------------------------
   //    Print the circle of confusion figure on the status line.
   //------------------------------------------------------------------

void PrintCC( void )
{
	char    buffer[20];

	WnPrintxy ( status_line, 22, 0, CreateCC ( buffer ) );

}

   //------------------------------------------------------------------
   //    Prints the Distance list that is in use on the status line.
   //------------------------------------------------------------------

void PrintDList( void )
{
    extern menu_entry dist_menu_entries[];

    WnPrintxy ( status_line, 65, 0, spaces13 );
    WnPrintxy ( status_line, 65, 0, dist_menu_entries[ distances_list ].str );
}

   //------------------------------------------------------------------
   //    Prints the F Stop list that is in use on the status line.
   //------------------------------------------------------------------


void PrintFSList( void )
{
    extern menu_entry fstop_menu_entries[];

    WnPrintxy ( status_line, 39, 0, spaces13 );
    WnPrintxy ( status_line, 39, 0, fstop_menu_entries[ fstop_list ].str );
}

BOOLEAN UpdateFstopList ( MENU * M )
{
    fstop_list = M -> copt;

    return FALSE;
}

BOOLEAN UpdateDistList ( MENU * M )
{
    distances_list = M -> copt;

    return FALSE;
}
