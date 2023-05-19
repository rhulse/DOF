//
//      Globals used in the DOF program. They are in dofmain.c
//
#if !defined(GLOBALS_H)
#define GLOBALS_H

typedef struct _settings
{
        char   *name;
        int     value;
}SETTINGS;

   // structures for directory lists

extern	int global_colours[];		// an array of colours

#define WIN_FGC 	global_colours[0]
#define WIN_BGC 	global_colours[1]
#define BORDER_FGC	global_colours[2]
#define DESKTOP_FGC	global_colours[3]
#define DESKTOP_BGC	global_colours[4]
#define HILITE_FGC	global_colours[5]
#define WARN_FGC	global_colours[6]
#define WARN_BGC	global_colours[7]
#define MESSAGE_FGC	global_colours[8]
#define MESSAGE_BGC	global_colours[9]
#define HELP_FGC	global_colours[10]
#define HELP_BGC	global_colours[11]
#define BAR_FGC 	global_colours[12]
#define BAR_BGC 	global_colours[13]

extern  float   focal_length;             //  Default Focal Length
extern  float   f_stop;                   //  Default F Stop
extern  float   cir_conf;                 //  Default Alowable circle of confusion in æm
extern  float   hyperfocal;               //  Hypefocal Distance
extern  float   near_limit;               //  some more globals
extern  float   far_limit;                //  some more globals
extern  float   focused_distance;         //  Default Focused Distance
extern  int     fstop_list;
extern  int     distances_list;

#endif
