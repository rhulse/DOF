//
//                      ERROR.C
//      This module contains the error handling routine.
//
//
#define NO_EXT_KEYS // extensions disabled
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <easyc.h>             // defines EQ LT GT etc
#include "window.h"
#include "screen.h"
#include "keyboard.h"
#include "dof.h"        // defines menu and help strings
#include "globals.h"

void Error ( void );

char    *err2 = "No such entry",          //  2
        *err9 = "Bad file name",          //  9
        *err12 = "No memory left",        // 12
        *err13 = "No access allowed",     // 13
        *err17 = "File allready exists",  // 17
        *err22 = "Invalid entry",         // 22
        *err24 = "File error",            // 24
        *err28 = "No space left",         // 28
        *errx  = "Unknown error.",        // not known
        *esc_str = "...Press <ESC> to continue.";

//-----------------------------------------------------------------
// Does the dos error handling ##
//-----------------------------------------------------------------

void Error ( void )
{
    extern  int error_flag;
    extern  int critical_error; // critical error flag set by wnint24
    extern  int vdu;
    WINDOW *error;
    char    buffer[60],
           *str;

    if ( NOT critical_error )   // critical error allready handled
    {
        VsBorderCol ( WARN_BGC );

        switch ( errno )
        {
            case ENOENT : str = err2;  break;   //  2
            case EBADF  : str = err9;  break;   //  9
            case ENOMEM : str = err12; break;   // 12
            case EACCES : str = err13; break;   // 13
            case EEXIST : str = err17; break;   // 17
            case EINVAL : str = err22; break;   // 22
            case EMFILE : str = err24; break;   // 24
            case ENOSPC : str = err28; break;   // 28
            default     : str = errx;  break;
        }
        strcpy ( buffer, str );
        strcat ( buffer, esc_str );

        error = Message( buffer, WARN_FGC, WARN_BGC );
//        WnBorderChg ( error, FALSE );   // dont change border when active

        Bleep();
        VsDisp();

        while ( GetAKey() NE ESC )
            ;
        WnKill ( error );

        VsBorderCol ( BORDER_FGC );
    }
    critical_error = FALSE;     // reset flags
    error_flag = FALSE;
}
