#include "OpenGL.h"
using namespace S_GL_Interfaces::GL_New;


#include "Engine.h"
extern S_Engine Engine;

#include "Functions.h"


RTT_New::RTT_New()
{		
	this->Rendering_To = 0;
	this->Render_Tex = 0;
	this->Render_FBO = 0;
	this->Render_DepthBuf = 0;
}

RTT_New::~RTT_New()
{
	//
}

bool RTT_New::Initialize()
{
	return true;
}

void RTT_New::Destroy()
{
	//
}										

void RTT_New::Start_Render(u_int nWidth, u_int nHeight, u_int To)//to render a image to a texture
{							   
	if(this->Render_FBO != NULL)//already rendering
		return;

	this->Rendering_To = To;									


	if(this->Rendering_To != 0)
		this->Render_Tex = Engine.Resources.Get_Tex( this->Rendering_To );
	else
		glGenTextures(1, &this->Render_Tex); 

	Engine.OpenGL_Assert( "Failed to configure render texture",4 );

	glPushMatrix();									

	
	// generate namespace for the frame buffer, colorbuffer and depthbuffer
	glGenFramebuffersEXT(1, &this->Render_FBO);			   
	Engine.OpenGL_Assert( "Failed to gen fbo",4 );
	glGenRenderbuffersEXT(1, &this->Render_DepthBuf);
	Engine.OpenGL_Assert( "Failed to gen render buffer",4 );
	//switch to our fbo so we can bind stuff to it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->Render_FBO);
	Engine.OpenGL_Assert( "Failed to bind fbo",4 );

	//create the colorbuffer texture and attach it to the frame buffer
	glBindTexture(GL_TEXTURE_2D, this->Render_Tex );
	Engine.OpenGL_Assert( "Failed to bind texture",4 );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_RGBA, GL_INT, NULL);
	Engine.OpenGL_Assert( "Failed to tex image",4 );

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, this->Render_Tex, 0);
	Engine.OpenGL_Assert( "Failed to attatch texture",4 );

	// create a render buffer as our depth buffer and attach it
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, this->Render_DepthBuf);
	Engine.OpenGL_Assert( "Failed to bind render buffer",4 );
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, nWidth, nHeight);
	Engine.OpenGL_Assert( "Failed to get render buffer storage",4 );
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, this->Render_DepthBuf);
	Engine.OpenGL_Assert( "Failed to attatch render buffer storage",4 );
	// Go back to regular frame buffer rendering
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->Render_FBO);


	std::string FBO_Status;
	GLenum Ret_Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch( Ret_Status )
	{
	case GL_FRAMEBUFFER_UNDEFINED:
		{
			FBO_Status="Undefined";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		{										  
			FBO_Status="Incomplete Attatchment";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		{				 
			FBO_Status="Incomplete Missing Attatchment";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		{			
			FBO_Status="Incomplete Draw Buffer";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		{				
			FBO_Status="Incomplete Read Buffer";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		{			
			FBO_Status="Incomplete Multisample";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		{				   
			FBO_Status="Incomplete Layer Targets";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		{			  
			FBO_Status="Incomplete Dimensions";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		{				 
			FBO_Status="Incomplete Formats";
		}break;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		{			  
			FBO_Status="Unsupported";
		}break;
	}

	if( FBO_Status.length() > 0 || Ret_Status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
			Engine.OpenGL_Assert("Frame buffer not complete, " + To_Str(Ret_Status) + ", " + FBO_Status,5);
	}



	// FBO render pass
	glViewport (0, 0, nWidth, nHeight);
	// set The Current Viewport to the fbo size glBindTexture(GL_TEXTURE_2D, 0);		  
	glViewport (0, 0, nWidth, nHeight);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->Render_FBO);
	glClearColor (.0f, .0f, .0f, .0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	// Framebuffer render pass		 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	sglEnable(GL_DEPTH_TEST);
}

u_int RTT_New::End_Render()//save render to texture and return ID
{						
	if(this->Render_FBO == NULL)//not trying to render
		return 0;
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	Engine.OpenGL_Assert( "Error after render",4 );  

	// Return to rendering to the main framebuffer instead of 'fbo'.
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );	
	Engine.OpenGL_Assert( "Failed to unbind fbo",4 );

   u_int Ret = 0;

	if(this->Rendering_To != 0)
	{
		Ret = this->Rendering_To;
	}
	else
	{
		Ret = Engine.Resources.Add_Tex(this->Render_Tex);//add to list of images 
	}

	this->Rendering_To = 0;
	this->Render_Tex = 0;
	this->Render_FBO = 0;
	this->Render_DepthBuf = 0;

	
	Engine.Application.Window.Reshape(Engine.Application.Window.Wind_w, Engine.Application.Window.Wind_h);

	glPopMatrix();

	return Ret;
}



Shader_New::Shader_New()
{			
	this->Shader_Prog=NULL;this->Shader_Vert=NULL;this->Shader_Frag=NULL;
}

Shader_New::~Shader_New()
{
	//
}

bool Shader_New::Initialize()
{							 
	Engine.OpenGL_Assert( "Pre Shader Error", 1 );

	this->Shader_Vert = glCreateShader(GL_VERTEX_SHADER);
	this->Shader_Frag = glCreateShader(GL_FRAGMENT_SHADER);

	int WentOk=GL_TRUE;

	std::string vv = Engine.Application.Input.Load_File_Str("shaders/vert.txt");
	const GLint vvLength=vv.length();						   
	const GLchar* vvChar=(GLchar*)vv.c_str();

	std::string ff = Engine.Application.Input.Load_File_Str("shaders/frag.txt");
	const GLint ffLength=ff.length();						   
	const GLchar* ffChar=(GLchar*)ff.c_str();

	glShaderSource(this->Shader_Vert, 1, &vvChar,&vvLength);
	if( Engine.OpenGL_Assert( "Shade Vert Source", 3) ) return false;
	
	//glShaderSource(APP.Shader_v, 1, (const GLchar**)&source, &length);
	glShaderSource(this->Shader_Frag, 1, &ffChar,&ffLength);   
	if( Engine.OpenGL_Assert( "Shade Frag Source", 3 ) ) return false;


	glCompileShader(this->Shader_Vert);							  
	glGetShaderiv(this->Shader_Vert,GL_COMPILE_STATUS,&WentOk);
	if(WentOk==GL_FALSE)									   
	{
		GLint Info_Len;
		glGetShaderiv(this->Shader_Vert,GL_INFO_LOG_LENGTH,&Info_Len);//get buffer length
		char* Buffer = new char[Info_Len+1];memset(Buffer,0,Info_Len);
		//char Buffer[512];memset(Buffer,0,512);
		glGetShaderInfoLog(this->Shader_Vert,Info_Len,&Info_Len,Buffer);


		std::string Err_Rep(Buffer);Err_Rep="Vert Shader Failed To Compile:\n"+Err_Rep;
		Engine.Register_Error(S_Error(Err_Rep,3));
		return false;
	}

	glCompileShader(this->Shader_Frag);
	glGetShaderiv(this->Shader_Frag,GL_COMPILE_STATUS,&WentOk);
	if(WentOk==GL_FALSE)				 
	{
		GLint Info_Len;
		glGetShaderiv(this->Shader_Frag,GL_INFO_LOG_LENGTH,&Info_Len);//get buffer length
		char* Buffer = new char[Info_Len+1];memset(Buffer,0,Info_Len);
		//char Buffer[512];memset(Buffer,0,512);
		glGetShaderInfoLog(this->Shader_Frag,Info_Len,&Info_Len,Buffer);


		std::string Err_Rep(Buffer);Err_Rep="Frag Shader Failed To Compile:\n"+Err_Rep;
		Engine.Register_Error(S_Error(Err_Rep,3));
		return false;
	}

	this->Shader_Prog = glCreateProgram();

	glAttachShader(this->Shader_Prog,this->Shader_Vert);
	glAttachShader(this->Shader_Prog,this->Shader_Frag);

	glLinkProgram(this->Shader_Prog);  
	glGetProgramiv(this->Shader_Prog,GL_LINK_STATUS,&WentOk);
	if(WentOk==GL_FALSE)						   
	{
		GLint Info_Len;
		glGetShaderiv(this->Shader_Prog,GL_INFO_LOG_LENGTH,&Info_Len);//get buffer length
		char* Buffer = new char[Info_Len+1];memset(Buffer,0,Info_Len);
		//char Buffer[512];memset(Buffer,0,512);
		glGetShaderInfoLog(this->Shader_Prog,Info_Len,&Info_Len,Buffer);


		std::string Err_Rep(Buffer);Err_Rep="Failed To Link Shader:\n"+Err_Rep;
		Engine.Register_Error(S_Error(Err_Rep,3));
		return false;
	}

	glUseProgram(this->Shader_Prog);
	
																					 
	this->Texture_Enabled=glGetUniformLocation(this->Shader_Prog,"Tex_Enabled");	   
	this->Lighting_Enabled=glGetUniformLocation(this->Shader_Prog,"Light_Enabled"); 
	this->Fog_Enabled=glGetUniformLocation(this->Shader_Prog,"Fog_Enabled"); 

	this->Ambient=glGetUniformLocation(this->Shader_Prog,"myAmbient");  			

	return true;
}

void Shader_New::Destroy()
{
	//
}

void Shader_New::Enable(u_int What)
{							 
	this->Change(What,true);
}

void Shader_New::Disable(u_int What)
{			  
	this->Change(What,false);
}

void Shader_New::Change(u_int What,u_int Value)
{		
	if( Engine.Application.OpenGL.Record_Bad_Change )
		Engine.OpenGL_Assert( "Change error BEFORE: " + To_Str(What) + ", " + To_Str(Value), 1 );
											
	/*this->Change_What_History.push_back( What );
	this->Change_Value_History.push_back( Value );
																	   
	if( this->Change_What_History.size() > 20 )
		this->Change_What_History.erase( this->Change_What_History.begin() );

	if( this->Change_Value_History.size() > 20 )
		this->Change_Value_History.erase( this->Change_Value_History.begin() );*/


	switch(What)
	{
	case GL_TEXTURE_2D:
		{
			glUniform1ui(this->Texture_Enabled,Value);	
		}break;
	case GL_VERTEX_ARRAY:
		{
			if( Value == TRUE )
				glEnableClientState( GL_VERTEX_ARRAY );
			else
			{
				glVertexPointer(2, GL_FLOAT, 0, NULL);
				glDisableClientState( GL_VERTEX_ARRAY );
			}
		}break;
	case GL_TEXTURE_COORD_ARRAY:
		{										 
			if( Value == TRUE )
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			else
			{
				glTexCoordPointer( 1, GL_FLOAT, 0, NULL );
				glDisableClientState( GL_TEXTURE_COORD_ARRAY );
			}
		}break;
	case GL_LIGHTING:
		{			
			glUniform1ui(this->Lighting_Enabled,Value);
		}break;
	case GL_FOG:
		{
			glUniform1ui(this->Fog_Enabled,Value);
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

void Shader_New::Change(u_int What,Vector3 Value)
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

void Shader_New::Shade_Ambient(float r,float g,float b,float a)
{						   
	glUniform4f(this->Ambient,r,g,b,a);
}

void Shader_New::Cull(bool Mode)
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

void Shader_New::Get_Log(GLuint Shader)
{
	int infologLength = 0;

	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(Shader, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(Shader, infologLength, &charsWritten, infoLog);

		std::string log = infoLog;

		free(infoLog);

		Engine.Application.Error_Log.Write_Log("Shader Log: " + log);
	}
}	