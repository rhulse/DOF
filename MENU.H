#if !defined(MENU_H)
#define MENU_H

typedef struct
{
    char               *str;            // the menu entry
    int                 hil;            // highlight position
    int                 help;           // helps to use
    unsigned int        hot_key;        // the entries `hot key'
    BOOLEAN  ( *function)( void * );    // function
}menu_entry;

typedef struct
{
    unsigned int        key;                    // the key that selects the
    BOOLEAN          ( *function)( void * );    // function with pointer to anything
}valid_key;

typedef struct      // structure for menu windows
{
    int         wid;            // width of window
    int         hgt;            // height of window
    int         x;              // x position on screen
    int         y;              // y position on screen
    int         fgc;            // fg colour of window
    int         bgc;            // bg colour of window
    int         copt;           // current menu option
    char       *title;          // the windows title
    int         offset;         // x offset of entries
    menu_entry *entries;        // entries and highlights etc.
    int         entry_count;    // number of menu entries
    valid_key   *keys;          // lookup table of keys pressed and functions
    int         key_count;      // number of keys in table
    WINDOW      *win_ptr;       // pointer to window
}MENU;

extern  int     RunPullDownMenu( void );      // run the menu module
extern  int     RunMenu( MENU * );      // run the menu module
extern  int     AppleMenu( void );      // `apple' menu .. uses menu module
extern  int     FilesMenu( void );      // file menu .. uses menu module
extern  int     CalcMenu( void );       // calculations menu
extern  int     DefaultMenu( void );    // default menu .. uses menu module
extern  int     FStopMenu( void );      // fstop menu .. uses menu module
extern  int     DistMenu( void );       // dist menu .. uses menu module
extern  MENU   *MakeAMenu( MENU * );    // makes a menu from a structure
extern  MENU   *MakeMainMenu( MENU * ); // makes the main menu from a structure
extern  void    DimMenu( MENU * );      // dims the hilite letters on menus
extern  void    DimPullDownMenu( MENU * ); // dims the hilite letters on pull down menu
extern  void    RestoreMenu( MENU * );  // restores the hilite letters on menus
extern  void    RestorePullDownMenu( MENU * );  // restores the hilite letters on menus

#endif
