#ifndef S_OPENGL_INTERFACE
#define S_OPENGL_INTERFACE


// for interfacing with non-supported parts of openGL

#include <vector>

#include "Core.h"
#include "myMath.h"
#include "GL\glew.h"
#include "GL\GL.h"

#define NULL 0

															 
#define sglEnable(X) Engine.Application.OpenGL.GL_Shader->Enable(X)
#define sglDisable(X) Engine.Application.OpenGL.GL_Shader->Disable(X)

	/*						  

struct VBO_Vert
{
	Vector3 Pos;
	Vector3 Norm;
	Vector2 Tex;
};		*/

namespace S_GL_Interfaces
{
	struct S_GL_RTT// render to texture
	{
		virtual bool Initialize() = 0;
		virtual void Destroy() = 0;
		
		virtual void Start_Render(u_int nWidth, u_int nHeight, u_int To = 0) = 0;//to render a image to a texture
		virtual u_int End_Render() = 0;//save render to texture and return ID
	};

	struct S_GL_VBO
	{
		S_GL_VBO();
		void Draw( u_char Flags = 0);// render the vbo

		void Make( u_int nVert_Count, bool Tex = true );
		void Delete();

		//void Get_From( E_Objects::O_Mesh::Mesh_Container* Mesh );
		void Build_VBO();
		
		u_int Vert_Count;
		//Vector3* Vertices;
		//Vector2* Tex_Coords;

		u_int VBO_Verts, VBO_TexCoords;

		u_char Triangle_Draw;
	};

	struct S_GL_Shader// includes GL Settings
	{
		virtual bool Initialize() = 0;
		virtual void Destroy() = 0;		


		virtual void Enable(u_int What) = 0;
		virtual void Disable(u_int What) = 0;
		virtual void Change(u_int What,u_int Value) = 0;
		virtual void Change(u_int What,Vector3 Value) = 0;


		virtual void Shade_Ambient(float r,float g,float b,float a) = 0;
		virtual void Cull(bool Mode) = 0;
	};

	struct S_GL_Interface
	{
		S_GL_Interface();
		~S_GL_Interface();

		bool Initialize();// set interfaces
		void Destroy();

		S_GL_RTT* GL_RTT;// render to texture
		S_GL_Shader* GL_Shader;// shaders-> vertex and frag	

		bool Use_New_Shaders, Use_New_RTT, Use_New_VBO;	 
		bool Record_Bad_Change;	


		bool VBO_Supported;
		std::vector< S_GL_VBO > VBOs;
		S_GL_VBO* Get_VBO( u_int Index );
		u_int Get_VBO( S_GL_VBO* Which );
	};


	namespace GL_Default
	{	  
		struct RTT_Default:public S_GL_RTT
		{					   
			RTT_Default();
			~RTT_Default();

			bool Initialize();
			void Destroy();
		
			void Start_Render(u_int nWidth, u_int nHeight, u_int To);//to render a image to a texture
			u_int End_Render();//save render to texture and return ID

		private:
			u_int Rendering_To;	   

			GLuint Render_Tex,Render_FBO,Render_DepthBuf;

		};
		struct Shader_Default:public S_GL_Shader
		{			
			Shader_Default();
			~Shader_Default();

			bool Initialize();
			void Destroy();			


			void Enable(u_int What);
			void Disable(u_int What);
			void Change(u_int What,u_int Value);
			void Change(u_int What,Vector3 Value);


			void Shade_Ambient(float r,float g,float b,float a);
			void Cull(bool Mode);
		};
	}

	namespace GL_New
	{			  
		struct RTT_New:public S_GL_RTT
		{					   
			RTT_New();
			~RTT_New();

			bool Initialize();
			void Destroy();
		
			void Start_Render(u_int nWidth, u_int nHeight, u_int To = 0);//to render a image to a texture
			u_int End_Render();//save render to texture and return ID			 

		private:
			u_int Rendering_To;	   

			GLuint Render_Tex,Render_FBO,Render_DepthBuf;
		};
		struct Shader_New:public S_GL_Shader
		{			
			Shader_New();
			~Shader_New();

			bool Initialize();
			void Destroy();			


			void Enable(u_int What);
			void Disable(u_int What);
			void Change(u_int What,u_int Value);
			void Change(u_int What,Vector3 Value);


			void Shade_Ambient(float r,float g,float b,float a);
			void Cull(bool Mode);	
												  
			std::vector< u_int > Change_What_History;
			std::vector< u_int > Change_Value_History;

		private:
			GLuint Shader_Prog,Shader_Vert,Shader_Frag;

			GLuint Lighting_Enabled,Texture_Enabled,Fog_Enabled;//STATES 
			GLuint Ambient;

			void Get_Log(GLuint Shader);
		};
	}
}



#endif