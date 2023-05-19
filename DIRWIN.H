#if !defined(DIRWIN_H)
#define DIRWIN_H

extern  int     RunFileWin( void * );
extern  BOOL    FileHomeKey ( void * );
extern  BOOL    FileEndKey ( void * );
extern  BOOL    FilePgDnKey ( void * );
extern  BOOL    FilePgUpKey ( void * );
extern  BOOL    FileDnKey ( void * );
extern  BOOL    FileUpKey ( void * );
extern  void    FHiliteOpt ( void * );
extern  void    FUnhiliteOpt ( void * );
extern  void    DoTopLine ( void * );
extern  void    DoBottomLine ( void * );
extern  BOOL    RunFileWin( void * );
extern  BOOL    PrintDir( void * );

#endif
