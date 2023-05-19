//
//                          PRINTER.C
//      This module contains the functions to print to a printer.
//
//
#include <dos.h>
#include <stdio.h>
#include <easyc.h>             //defines EQ LT GT etc
#include "keyboard.h"
#include "window.h"
#include "screen.h"
#include "mylib.h"
#include "globals.h"
#include "dof.h"        //defines menu and help strings

BOOL    PrintFile( char * );
static  int     DoPrint( char * );
static  int     PrintRetry( void );
static  int     PrintPause( void );
static  int     PrintString( char * );
static  int     PrintChar( char );

char    printingmessage[] = " Printing file ....",

        printerrormessage[] =
            "      Error! Printer not ready.\n"
            "Press <Esc> to quit, or <Enter> to re-try.",

        printpausemessage[] =
            " Printing paused. Press <Esc> to quit,\n"
            "    or any other key to continue.",

        reset_printer[] = "\x1b@",
        form_feed[]     = "\x0C";

static WINDOW *print_win;

   //-------------------------------------------------------------------
   //            Print function
   //-------------------------------------------------------------------

BOOL PrintFile( char *source )
{
    int     status,
            loop = TRUE;

    print_win = WnMake ( 43, 2, 17, 13, MESSAGE_FGC, MESSAGE_BGC );
    VsDisp();
    WnPrintxy ( print_win, 2, 0, printingmessage );
    WnOpen ( print_win );

    while ( loop )
    {
        status = BiosPrintStatus();         // if there is .....
        if ( ( NOT  ( status BAND 8 ) )     // not an error
        AND  (       status BAND 16   )     // and the printer is selected
        AND  ( NOT  ( status BAND 32 ) )    // and there is NOT no paper
        AND  (       status BAND 128 ) )    // and it isnt busy
        {
            VsDisp();
            if ( DoPrint ( source ) )
            {
                Error();
            }
            loop = FALSE;
        }
        else
        {
            Bleep();                 //if any error then tell the user and
            loop = PrintRetry();    //give then a chance to try again
        }
    }
    WnKill ( print_win );
    return ( TRUE );
}

   //-------------------------------------------------------------------
   //           Does the actual printing
   //-------------------------------------------------------------------

static  int DoPrint ( char *file )
{
    int     loop = TRUE;
    char    buffer[1024];
    FILE   *source;

    if ( ( source = fopen ( file, "rt" ) ) EQ NULL )
    {
        return ( TRUE );    // error
    }
    else
    {
        while ( ( fgets ( buffer, 1024, source ) ) AND loop )
        {
            loop = PrintString ( buffer );
        }
        fclose ( source );              //close the source
    }
    return ( FALSE ) ;      // no error
}

static  int PrintString ( char *string )
{
    int     loop = TRUE;

    while ( ( loop ) AND ( *string ) )
    {
        loop = PrintChar ( *string );
        string++;
    }
    return ( loop );
}

static  int PrintChar ( char character )
{
    int     result,
            main_loop = TRUE,
            retry_loop = TRUE;
		
    while ( retry_loop )
    {
        if ( KeyPressed() )
        {
            main_loop = retry_loop = PrintPause();
            VsDisp();
        }

        if ( retry_loop )
        {
            result = BiosPrintChar ( character );
            if ( ( result BAND 1 )      //timeout
              OR ( result BAND 8 )      //i/o error
              OR ( result BAND 32 ) )   //paper out
            {
                main_loop = retry_loop = PrintRetry();
                VsDisp();
            }
            else
                retry_loop = FALSE;
        }
    }
    return ( main_loop );
}

static  int PrintRetry()
{
    int     loop = TRUE;

    WnCls( print_win, 0, print_win -> hgt -1 );

    WnPrintxy ( print_win, 0, 0, printerrormessage );

    WnChgAttr ( print_win, 6, 0, V_LTRED, V_BLACK, 6 );  // error
    WnChgAttr ( print_win, 7, 1, V_LTBLUE, V_BLACK, 5 ); // <ESC>
    WnChgAttr ( print_win, 25, 1, V_LTBLUE, V_BLACK, 7 );//<ENTER>

    VsDisp();

    if ( GetAKey() EQ ENTER )    // else reprint the message 
    {
        WnCls( print_win, 0, print_win -> hgt -1 );
        WnPrintxy ( print_win, 2, 0, printingmessage );
    }
    else                        
        loop = FALSE;           // abort then do nothing
    return ( loop );
}

static  int PrintPause()
{
    int     loop = TRUE;
        
    WnCls( print_win, 0, print_win -> hgt -1 );

    WnPrintxy ( print_win, 1, 0, printpausemessage );

    WnChgAttr ( print_win, 24, 0, V_LTBLUE, V_BLACK, 5 );    // <ESC>

    VsDisp();
    GetAKey();           // throw away the key that was pressed

    if ( GetAKey() EQ ESC )     // abort
    {
        loop = FALSE;
        PrintString ( reset_printer );  // stop it printing fast
        PrintChar ( FF );               //and do a form feed
    }
    else                        // else reprint the message
    {
        WnCls( print_win, 0, print_win -> hgt -1 );
        WnPrintxy ( print_win, 2, 0, printingmessage );
    }
    return ( loop );
}
