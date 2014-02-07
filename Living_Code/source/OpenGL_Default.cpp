#include "OpenGL.h"
using namespace S_GL_Interfaces::GL_Default;

#include "Engine.h"
extern S_Engine Engine;

RTT_Default::RTT_Default()
{
	//
}

RTT_Default::~RTT_Default()
{
	//
}

bool RTT_Default::Initialize()
{
	return true;
}

void RTT_Default::Destroy()
{
	//
}										

void RTT_Default::Start_Render(u_int nWidth, u_int nHeight, u_int To)//to render a image to a texture
{
	//
}

u_int RTT_Default::End_Render()//save render to texture and return ID
{
	return 0;
}					 



Shader_Default::Shader_Default()
{
	//
}

Shader_Default::~Shader_Default()
{
	//
}

bool Shader_Default::Initialize()
{
	return true;
}

void Shader_Default::Destroy()
{
	//
}

void Shader_Default::Enable(u_int What)
{
	this->Change(What,true);
}

void Shader_Default::Disable(u_int What)
{			   
	this->Change(What,false);
}

#include "Functions.h"

void Shader_Default::Change(u_int What,u_int Value)
{
	switch(What)
	{  
	case GL_VERTEX_ARRAY:
		{
			if( Value == TRUE )
				glEnableClientState( GL_VERTEX_ARRAY );
			else
				glDisableClientState( GL_VERTEX_ARRAY );
		}break;
	case GL_TEXTURE_COORD_ARRAY:
		{										 
			if( Value == TRUE )
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			else
				glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		}break;
	case GL_LIGHTING:
		{												
			if(Value)
			{
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
			}
			else
			{
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
			}
		}break;	
	default:// assume enableing or disabling
		{												 
			if( Value == TRUE )
				glEnable( What );
			else
				glDisable( What );
		}break;
	}																			  
					
	if( Engine.Application.OpenGL.Record_Bad_Change )
		Engine.OpenGL_Assert( "Change error: " + To_Str(What) + ", " + To_Str(Value), 1 );
}

void Shader_Default::Change(u_int What,Vector3 Value)
{
	switch(What)
	{
	case GL_LIGHTING:
		{								
			GLfloat lightpos[] = {Value.x, Value.y, Value.z};
			glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
		}break;
	}
}

void Shader_Default::Shade_Ambient(float r,float g,float b,float a)
{
	//
}

void Shader_Default::Cull(bool Mode)
{  
	if(Mode)
	{
		sglEnable(GL_CULL_FACE);
		glPolygonMode(GL_BACK,GL_POINT);	
	}
	else
	{
		sglDisable(GL_CULL_FACE);
		glPolygonMode(GL_BACK,GL_FILL);	
	}
}