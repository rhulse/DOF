#if !defined(FILE_H)
#define FILE_H

extern  BOOL    CopyFile( char * );
extern  int     DoCopy( char * , char * );
extern  void    MakeDest ( char * , char * , char * );
extern  int     CheckIfSame ( char * , char * );
extern  long    GetSpace ( char * );
extern  BOOL    MoveFile( char * );
extern  int     DoMove( char * , char * );
extern  BOOL    DeleteFile( char * );

#endif
