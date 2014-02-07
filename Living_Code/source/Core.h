#ifndef E_CORE
#define E_CORE

//Functionality
#define USE_XBOX//to build with XBOX functionality

//#define PAUSE_DRAW_WHEN_UPDATE // to pause the drawing thread when deleting objects to avoid memory errors


typedef unsigned int u_int;
typedef unsigned char u_char;

#define APP_NAME "Living Code, by Bennett Steele"



//ticks per second, 0 == no cap
#define ENGINE_TPS 60
#define DRAW_TPS 60
#define LOGIC_TPS 1024

//  PHYSICS

#define P_ADD 0.01f
#define P_DRAG .02f
#define P_DRAG_1 0.05f

//engine


// GAME STUFF


#endif