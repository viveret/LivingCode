#include "Draw_Utility.h"
using namespace Draw_Utility;

#include "Engine.h"
extern S_Engine Engine;	 

#define ScreenW Engine.Application.Window.Wind_w
#define ScreenH Engine.Application.Window.Wind_h

void Draw_Utility::String_Draw(std::string txt,int x,int y,float Scale,float r,float g,float b,float a)
{
	glPushMatrix();

    glColor4f(r,g,b,a);
	Engine.Resources.Use_Texture( Engine.Resources.Text );



	float Tx=1.f/16.f,Ty=1.f/8.f;
	float Length=txt.length()*8.f;
    for(unsigned int l=0;l<txt.length();l++)
    {
		POINT Pos;Pos.y=unsigned int(txt[l])/16;Pos.x=unsigned int(txt[l])-16*Pos.y;

		float L=l-txt.length()/2.f;

		float nx = x+8.f+L*16.0f*Scale+Length,
			  ny = y+8.f-8.f*Scale,
			  ns = 8.f*Scale;



        glBegin(GL_QUADS);
			glTexCoord2f(Pos.x*Tx,Pos.y*Ty);      glVertex2f(nx - ns,ny+8.f+ns);
            glTexCoord2f(Pos.x*Tx+Tx,Pos.y*Ty);   glVertex2f(nx + ns,y+8.f+ns);
            glTexCoord2f(Pos.x*Tx+Tx,Pos.y*Ty+Ty);glVertex2f(nx + ns,y+8.f-ns);
            glTexCoord2f(Pos.x*Tx,Pos.y*Ty+Ty);   glVertex2f(nx - ns,y+8.f-ns);
        glEnd();
    }
    glColor4f(1,1,1,1);

	glPopMatrix();
}

void Draw_Utility::String_Draw(std::string txt,int x,int y,float r,float g,float b,float a)
{
	Draw_Utility::String_Draw(txt,x,y,1.f,r,g,b,a);
}							




void Draw_Quad( Vector2 Pos, Vector2 Size )
{											 
	glBegin( GL_QUADS );   
		glVertex2f( Pos.x, Pos.y );
		glVertex2f( Pos.x, Pos.y + Size.y );
		glVertex2f( Pos.x + Size.x, Pos.y + Size.y );
		glVertex2f( Pos.x + Size.x, Pos.y );
	glEnd();
}

void Draw_Quad( float x, float y, float w, float h )
{
	Draw_Quad( Vector2(x, y), Vector2(w, h) );
}
