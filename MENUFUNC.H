//      Functions called by menu_main , menu_
#if !defined(MENUFUNC_H)
#define MENUFUNC_H

extern BOOL     Option0( void );        // single entry option
extern BOOL     Option1( void );        // make a DOF chart

//      Functions called by all `static' menus

extern void    MainSetNewCopt ( void * , int );
extern void    SetNewCopt ( void * , int );
extern BOOL    CallPullDownCopt  ( void * );
extern BOOL    CallCurrentOption ( void * );
extern BOOL    CallFileMenuCopt ( void *);
extern BOOL    HomeKey ( void * );     // processes the home arrow key
extern BOOL    EndKey ( void * );      // processes the end arrow key
extern BOOL    DnKey ( void * );       // processes the dn arrow key
extern BOOL    UpKey ( void * );       // processes the up arrow key
extern BOOL    LeftKey ( void * );       // processes the left arrow key
extern BOOL    RightKey ( void * );       // processes the right arrow key
extern void    HiliteOpt ( void * );
extern void    UnhiliteOpt ( void * );
void MainHiliteOpt ( void * );
void MainUnhiliteOpt ( void * );
void    HiliteLetter ( void * , int );
void    UnhiliteLetter ( void * , int );
BOOLEAN ReturnTrue ( void );
BOOLEAN ReturnFalse ( void );

#endif
