//*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
//                             DOF.H                                        *
//    This include file contains various definitions for DOF.               *
//                                                                          *
//                                                                          *
//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
#if !defined(DOF_H)
#define DOF_H

//structures for directory lists

struct directory
    {
        char   file_name[13];
        struct directory *prev;
        struct directory *next;
    };

// chart.c functions

void    ChartDofs( void );
void    MakeChart( float * , int * );
char   *MakeDofFileName( char * );
void    MakeTitleLines( char * , char * , char * );
char   *MakeTopLine( int * , int , char * );
int     MakeResultLines( int * , int , char * , char * );
char   *MakeHDLine( int * , int , char * );
int     CheckFile (  char * );
char   *FindReplace( char * , char , char );

// console.c functions

extern  WINDOW *CreateWait( void );
extern  float   GetInput( WINDOW * , int , int , float , float );
extern  float   CheckInput( char *, float , float );
extern  int     PromptForValues( int );
extern  void    PrintSingle( void );
extern  char   *FloatToDist ( float , char * );
extern  void    MakeResultStrings( char * , char * , char * );
extern  void    CommandLine( char ** );
extern  WINDOW *Message( char * , int , int );
extern  int     Question ( char * , int , int );
extern  int     GetDest( char * );

// dofmain.c functions

extern  int     GetAKey( void );
extern  void    Bleep( void );

// error.c functions

extern  void    Error( void );

// misc.c functions

extern  void    DofCalc( void );
extern  void    PauseWin( void );     // holds things up a while

// time.c functions

extern  void    InitTime( void );
extern  void    DoTime( void );

// dwnedit.asm function

int     DWnEdit ( WINDOW * , int , int , int , char * );

// dofint28.asm functions

void    StealInt28( void );     // steal DOS idle interupt
void    RestoreInt28( void );   // restore DOS idle interupt

#endif
