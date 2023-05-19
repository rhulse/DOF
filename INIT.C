//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
//                                                                          *
//   Set up the DOF program globals and windows and everything !!!!         *
//                                                                          *
//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
#define NO_EXT_KEYS     // extensions disabled
#include <easyc.h>      // defines EQ LT GT etc
#include "mylib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include "screen.h"
#include "globals.h"
#include "status.h"
#include "keyboard.h"
#include "dof.h"        // defines menu and help strings
#include "menu.h"
#include "help.h"

void    Init( void );       // does initialisations
static	void SetUpGlobals ( void );
static	void GetMyGlobals( void );
static	void GetFileGlobals( void );
static  int GetColourValue ( char * );
static  int GetAccessValue ( char * );
static  char *rtrim ( char *str );
//
//  colours used by dof
//
static SETTINGS DofSettings[]=
{
    { "WindowFGC",          V_CYAN      },
    { "WindowBGC",          V_BLACK     },
    { "BorderColour",       V_MAGENTA   },
    { "DesktopFGC",         V_LTBLUE    },
    { "DesktopBGC",         V_BLACK     },
    { "LetterColour",       V_LTWHITE   },
    { "WarnFGC",            V_LTWHITE   },
    { "WarnBGC",            V_RED       },
    { "MessageFGC",         V_MAGENTA   },
    { "MessageBGC",         V_BLACK     },
    { "HelpFGC",            V_BLACK     },
    { "HelpBGC",            V_GREEN     },
    { "HilightBarFGC",      V_YELLOW    },
    { "HilightBarBGC",      V_MAGENTA   },
    { "ScreenAccess",       DMA       },    // from here on are extra settings !
    { "CircleOfConfusion",  33        }
};
#define extra_settings 2
#define num_settings  ( sizeof ( DofSettings ) / sizeof ( SETTINGS ) )
#define num_colour_settings  num_settings - extra_settings
#define access_setting num_settings - 2

int global_colours[ num_colour_settings ];      // an array of colours

void Init( void )
{
//    extern int hide_cursor;
//    hide_cursor = FALSE;    // debugging

    // initialize the windows module
    WnInit();

	// set up colours and other bits for DOF to know about
    SetUpGlobals ();

    // colours for edge of boxes
    WnSetActColours ( HILITE_FGC, WIN_BGC );   

    // steal critical error int24
    WnStealInt24();                             

    // do a `desktop' look
    VsBackground ( DESKTOP_FGC, DESKTOP_BGC, '²' );

    // this is ignored if unsupported
    VsBorderCol ( BORDER_FGC );             

    // an area for programme data
    CreateStatusLine();     

    // shareware pause win
    PauseWin();

    // set the clock going
    InitTime();             
}

static  void SetUpGlobals ( void )
{
    GetMyGlobals ();
    GetFileGlobals();
};

static	void GetMyGlobals( void )
{
	int 	i;
	extern	int 	video_method;	// use direct screen writes


    for ( i = 0; i LT num_colour_settings; i++ )
	{
        global_colours [i] = DofSettings[i].value;
	}
    video_method = DofSettings[ access_setting ].value;

};

static	void GetFileGlobals( void )
{
    extern  int     video_method;   // method of screen access
    FILE    *fp;
    int     i,
            colour;
    char    buf[ 80 ],
            *value,
            *p;

    if ( NOT ( fp = fopen ( "DOF.INI" , "rt" ) ) )
        return;

    while( fgets ( buf, 80, fp ) )
    {
        buf[ strlen ( buf ) -1 ] = '\0';    // replace \n with NULL

        if( p = strchr( buf, ';' ) )        // remove comments
            *p = '\0';
        rtrim( buf );                       // remove spaces

        if ( NOT strlen ( buf ) OR ( NOT ( value = strchr ( buf, '=' ) ) ) )
            continue;
        *value++ = '\0';

        for ( i = 0; i LT num_colour_settings; i++ )
        {
            if ( NOT strcmpi ( buf, DofSettings[i].name ) )
            {
                if ( ( colour = GetColourValue( value ) ) NE FAIL );
                    DofSettings[i].value = colour;
                global_colours [i] = DofSettings[i].value;
            }
        }
        // check for non - colour settings
        if ( NOT strcmpi ( buf, DofSettings[ access_setting ].name ) )
        {
            if ( ( colour = GetAccessValue( value ) ) NE FAIL );
                video_method = DofSettings[ access_setting ].value = colour;
        }
    }
    fclose(fp);
}

static int GetColourValue ( char *string  )
{
	int 	i;
    static char *colour_strings[]=
    {
        "BLACK", "BLUE",      "GREEN",  "CYAN",   "RED",     "MAGENTA",
        "BROWN", "WHITE",     "GREY",   "LTBLUE", "LTGREEN", "LTCYAN",
        "LTRED", "LTMAGENTA", "YELLOW", "LTWHITE"
    };

    for ( i = 0; i LT 16; i++ )
	{
        if ( NOT stricmp ( string , colour_strings[i] ) )
			return ( i );
	}
	return ( FAIL );
}

static int GetAccessValue ( char *string  )
{
	int 	i;
    static char *access_strings[]=
    {
        "NULL", "BIOS", "NO_SNOW", "DMA"
    };

    for ( i = 0; i LT 4; i++ )
	{
        if ( NOT stricmp ( string , access_strings[i] ) )
			return ( i );
	}
	return ( FAIL );
}

//
//  remove any trailing spaces
//
static char *rtrim ( char *str )
{
    char *p = strrchr( str, ' ');

    if ( NOT p )
        return str;

    while( *p EQ ' ' AND p GT str )
        p--;
    p++;
    *p = '\0';
    return str;
}
