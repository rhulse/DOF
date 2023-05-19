#if !defined(STATUS_H)
#define STATUS_H

extern  void    CircConf( void );   // handles menu option
extern  void    PrintCC( void );    // print circle of conf in title window
extern  char   *CreateCC ( char * ); // creates asciiz of circle of conf
extern  void    PrintDList( void );
extern  void    PrintFSList( void );
extern  void    CreateStatusLine( void );
extern  void    UpdateFstopList ( void * );
extern  void    UpdateDistList ( void * );

#endif
