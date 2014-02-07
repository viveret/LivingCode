#ifndef DRAW_UTILITY
#define DRAW_UTILITY

#include <string>

#include "myMath.h"


namespace Draw_Utility
{			  
    void String_Draw(std::string txt,int x,int y,float Scale,float r,float g,float b,float a);
	void String_Draw(std::string txt,int x,int y,float r,float g,float b,float a);

}

															
void Draw_Quad( Vector2 Pos, Vector2 Size );
void Draw_Quad( float x, float y, float w, float h );

						 
												
#define ScreenW (float)Engine.Application.Window.Wind_w
#define ScreenH (float)Engine.Application.Window.Wind_h
					 

#endif