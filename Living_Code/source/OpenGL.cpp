#include "OpenGL.h"
using namespace S_GL_Interfaces;

#include <vector>
#include "Engine.h"
extern S_Engine Engine;




S_GL_VBO::S_GL_VBO()
{
	this->Vert_Count = 0;
	//this->Vertices = NULL;
	//this->Tex_Coords = NULL;

	this->VBO_Verts = 0;
	this->VBO_TexCoords = 0;

	this->Triangle_Draw = GL_TRIANGLES;
}

void S_GL_VBO::Draw( u_char Flags )
{			
	glPushMatrix();

	/*Engine.Application.OpenGL.GL_Shader->Enable( GL_VERTEX_ARRAY );
	Engine.Application.OpenGL.GL_Shader->Enable( GL_TEXTURE_COORD_ARRAY );

	if( Engine.Application.OpenGL.VBO_Supported )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->VBO_Verts );
		glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );       // Set The Vertex Pointer To The Vertex Buffer
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->VBO_TexCoords );
		glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );     // Set The TexCoord Pointer To The TexCoord Buffer
	}
	else
	{
		glVertexPointer( 3, GL_FLOAT, 0, this->Vertices ); // Set The Vertex Pointer To Our Vertex Data
		glTexCoordPointer( 2, GL_FLOAT, 0, this->Tex_Coords );  // Set The Vertex Pointer To Our TexCoord Data
	}

	glDrawArrays( GL_TRIANGLES, 0, this->Vert_Count );        // Draw All Of The Triangles At Once

					
	Engine.Application.OpenGL.GL_Shader->Disable( GL_VERTEX_ARRAY );
	Engine.Application.OpenGL.GL_Shader->Disable( GL_TEXTURE_COORD_ARRAY );

						 */
	if( this->VBO_Verts == 0 )// did not build the VBO
	{
		sglEnable( GL_VERTEX_ARRAY );
		//glEnableClientState(GL_COLOR_ARRAY);

		//glColorPointer(3, GL_FLOAT, 0, (float*)&this->Vertices);
		/*glVertexPointer(3, GL_FLOAT, 0, (float*)this->Vertices);

		if( this->Tex_Coords )						  
		{
			sglEnable( GL_TEXTURE_COORD_ARRAY );	 
			glTexCoordPointer( 2, GL_FLOAT, 0, (float*)this->Tex_Coords );
		}

		glDrawArrays( this->Triangle_Draw, 0, sizeof(Vector3) / sizeof(float) * this->Vert_Count );

		//glDisableClientState(GL_COLOR_ARRAY);		   	 
		if( this->Tex_Coords )	
			sglDisable( GL_TEXTURE_COORD_ARRAY );
									   */
		sglDisable( GL_VERTEX_ARRAY );
	}
	else// built VBO
	{
		//
	}

	glPopMatrix();
}

void S_GL_VBO::Make( u_int nVert_Count, bool Tex )
{
	/*if( this->Vertices == NULL )
	{
		this->Vert_Count = nVert_Count;

		this->Vertices = new Vector3[ nVert_Count ];

		if( Tex )
			this->Tex_Coords = new Vector2[ nVert_Count ];
	} */
}

void S_GL_VBO::Delete()
{		 
	/*if( this->Vertices != NULL )
	{
		this->Vert_Count = 0;
												   
		delete[] this->Vertices; this->Vertices = NULL;

		if( this->Tex_Coords != NULL )
		{
			delete[] this->Tex_Coords; this->Tex_Coords = NULL;
		}
	} */
}					   

/*void S_GL_VBO::Get_From( E_Objects::O_Mesh::Mesh_Container* Mesh )
{
	//
}*/

void S_GL_VBO::Build_VBO()
{	
	//if( Engine.Application.OpenGL.VBO_Supported && this->Vertices != NULL )
	{				
		/*Engine.Application.OpenGL.GL_Shader->Enable( GL_VERTEX_ARRAY );
		Engine.Application.OpenGL.GL_Shader->Enable( GL_TEXTURE_COORD_ARRAY );	

		Engine.OpenGL_Assert( "Could not gen ARB vert buffer", 1 );

		// Generate And Bind The Vertex Buffer
		glGenBuffersARB( 1, &this->VBO_Verts );                  // Get A Valid Name
		Engine.OpenGL_Assert( "Could not gen ARB vert buffer", 1 );

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->VBO_Verts );         // Bind The Buffer
		Engine.OpenGL_Assert( "Could not bind ARB vert buffer", 1 );
		// Load The Data
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, this->Vert_Count*3*sizeof(float), this->Vertices, GL_STATIC_DRAW_ARB );
		Engine.OpenGL_Assert( "Could not load ARB vert buffer", 1 );
 
		// Generate And Bind The Texture Coordinate Buffer
		glGenBuffersARB( 1, &this->VBO_TexCoords );                 // Get A Valid Name
		Engine.OpenGL_Assert( "Could not gen ARB tex buffer", 1 );

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->VBO_TexCoords );        // Bind The Buffer
		Engine.OpenGL_Assert( "Could not bind ARB tex buffer", 1 );
		// Load The Data
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, this->Vert_Count*2*sizeof(float), this->Tex_Coords, GL_STATIC_DRAW_ARB );
		Engine.OpenGL_Assert( "Could not load ARB tex buffer", 1 );
 
		// Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card		   
												   
		delete[] this->Vertices; this->Vertices = NULL;
		delete[] this->Tex_Coords; this->Tex_Coords = NULL;	 

		Engine.Application.OpenGL.GL_Shader->Disable( GL_VERTEX_ARRAY );
		Engine.Application.OpenGL.GL_Shader->Disable( GL_TEXTURE_COORD_ARRAY );	 */
	}
}




S_GL_Interface::S_GL_Interface()
{
	this->GL_RTT = NULL;
	this->GL_Shader = NULL;

	this->VBO_Supported = false;
	this->Use_New_Shaders = true;
	this->Use_New_RTT = true;
	this->Use_New_VBO = true;

	this->Record_Bad_Change = false;
}

S_GL_Interface::~S_GL_Interface()
{
	this->Destroy();
}

bool S_GL_Interface::Initialize()
{					
	Engine.Application.Main_Log.Write_Log("OpenGL Version: " + std::string((char*)glGetString(GL_VERSION)) );

	GLenum glewWentOK=glewInit();
	Engine.Application.Main_Log.Write_Line("GLEW Version: " + std::string((char*)glewGetString(GLEW_VERSION)) );



	std::vector<std::string> Not_Supported;
	std::vector<std::string> Is_Supported;


	if (glewGetExtension("GL_ARB_fragment_shader") == GL_TRUE && glewGetExtension("GL_ARB_vertex_shader") == GL_TRUE && this->Use_New_Shaders)
	{
		Is_Supported.push_back("custom shaders");
		this->GL_Shader = new GL_New::Shader_New;
	}
	else
	{
		Not_Supported.push_back("custom shaders");
		this->GL_Shader = new GL_Default::Shader_Default;
	}


	if (glewGetExtension("GL_ARB_framebuffer_object") == GL_TRUE && glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_UNSUPPORTED && this->Use_New_RTT)	
	{
		Is_Supported.push_back("frame buffers");
		this->GL_RTT = new GL_New::RTT_New;
	}
	else
	{
		Not_Supported.push_back("frame buffers");
		this->GL_RTT = new GL_Default::RTT_Default;
	}


	if (glewGetExtension("GL_ARB_vertex_buffer_object") == GL_TRUE && this->Use_New_VBO )	
	{
		Is_Supported.push_back("vertex buffers");
		this->VBO_Supported = true;
	}
	else
	{
		Not_Supported.push_back("vertex buffers");
		this->VBO_Supported = false;
	}


				
	Engine.Application.Main_Log.Write_Line("NOT Supported:");

	for(u_int i = 0;i < Not_Supported.size(); i++)									
		Engine.Application.Main_Log.Write_Line("\t" + Not_Supported[i] );

	Engine.Application.Main_Log.Write_Line("IS Supported:");

	for(u_int i = 0;i < Is_Supported.size(); i++)									
		Engine.Application.Main_Log.Write_Line("\t" + Is_Supported[i] );


				 
	if( this->GL_Shader->Initialize() == false )//failed
	{
		Engine.Register_Error(S_Error("Failed To Initialize shader Interface",3));return false;
	}

	if( this->GL_RTT->Initialize() == false )//failed
	{
		Engine.Register_Error(S_Error("Failed To Initialize fbo Interface",3));return false;
	}

	//made it to here, successfully initialized
	return true;
}

void S_GL_Interface::Destroy()
{
	if( this->GL_RTT )
	{
		delete this->GL_RTT;
		this->GL_RTT = NULL;
	}

	if( this->GL_Shader )
	{
		delete this->GL_Shader;
		this->GL_Shader = NULL;
	}

	this->VBO_Supported = false;
}

S_GL_VBO* S_GL_Interface::Get_VBO( u_int Index )
{
	return &this->VBOs[Index];
}

u_int S_GL_Interface::Get_VBO( S_GL_VBO* Which )
{
	for(u_int i = 0;i < this->VBOs.size(); i++)
		if( &this->VBOs[i] == Which )
			return i;

	return 0;
}