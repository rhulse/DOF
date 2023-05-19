#if !defined(DIR_H)
#define DIR_H

extern struct directory  *first , *node , *new , *top_line , *bottom_line;

extern  int     GetDirectory( void );
extern  void    KillDirEntry( void * );
extern  void    KillAllEntries( void );

extern  BOOL    SelectDofFileName( char * , char * );
extern  int     GetDofDirectory( void );

#endif
