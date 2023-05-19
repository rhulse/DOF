#if !defined(HELP_H)
#define HELP_H

#include <easyc.h>

extern  BOOL    Help( void * );         // General help for inside programme
extern  void    CLHelp ( void );        // command line help
extern  void    DoHelp ( WORD );        // viewing window help
extern  BOOL    AboutDof ( void );      // copyright info

#define VIEW_HELP       11
#define SELECT_HELP     12


#endif
