#ifndef ENGINE_APPLICATION
#define ENGINE_APPLICATION
							 

#include "Core.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <time.h> 		 
#include "GL\glew.h"
#include "GL\GL.h"

#include "Debug.h"
#include "XBox.h"
#include "myMath.h"
#include "OpenGL.h"

namespace E_Application//engine application
{			
	struct A_Window//win32 window
	{
		A_Window();
		~A_Window();

		bool Is_Active;//running

		void Create_Window(HINSTANCE nhInstance);
		void Destory_Window();
		void Reshape(unsigned int w,unsigned int h);
		void Move(int x,int y);

		unsigned int Wind_w,Wind_h;
		int Wind_x,Wind_y;

		bool Fullscreen,Focus;

		HGLRC hEng_RC,hDraw_RC;//  rendering context, links opengl calls to engine and draw threads
		HDC hDC;//    device context         draw to the window
	    HWND hWnd;//  window handle
		HINSTANCE hInstance;//               instance of application
	};

	struct Key
	{
		Key();
		u_int Key_Code;
		bool Down;

		void Update();
	};

	struct A_Input_Keys//key input
	{
		A_Input_Keys(std::string File);//try loading from file
		A_Input_Keys();//default

		void Load_Keymap(std::string File);//load keymap from file
		void Defaults();//defaults


		void Update(u_int Key_Code,bool Set);
		//GENERAL
		Key Quit;//quit the application
		Key Commands;
		//MENU
		Key Pause,To_Backpack,To_Map;

		//GAMEPLAY
		Key Forward,Backward,Left,Right;//movement
		Key Eat, Toggle_Think;
		Key Jump,Crouch,Run;
		Key Zoom;

		Key Primary_Fire,Secondary_Fire;//shooting
		Key Drop_Item,Reload;
		Key Chat;
	};					   

	struct Raw_Buffer
	{
		Raw_Buffer();
		void Delete();

		char* my_Buf;
		u_int Length;
	};

	struct A_Input//regular input
	{					  
		A_Input();
		~A_Input();
												
		/*std::string Load_File(std::string File);
		Raw_Buffer Get_Raw_File(std::string File);

		bool Get_XML_File(XML_File* File, std::string Dest);

		std::string Get_ZIP_File( std::string File );*/
		
		std::string Load_File_Str( std::string File, bool Silent = false );//load raw file, can be silent = no log
		Raw_Buffer Load_File( std::string File, bool Silent = false );//load raw file, can be silent = no log




		bool Typing;//is typing
		
		XBOXController XBOX;
		A_Input_Keys Keys;

		void Mouse_Move(unsigned int x,unsigned int y);
		unsigned int Mouse_X,Mouse_Y;
		int Mouse_Scroll;
		bool Mouse_L_Down,Mouse_R_Down;//menu

		//void Get();//gets all virtual key stuff
		//bool Is_Down(int Key_Code);//check if is down
	};
	struct A_Log
	{			
		A_Log();
		~A_Log();
		bool Open(std::string Where);

		void Write(std::string What);// without any change
		void Write_Line(std::string What);// some change, only per line
		void Write_Log(std::string What);// changes everything

		void Print_Time();// log time

		void Close();

		void Clear_History();//clear log history
		bool Record_History;//if wanting to record it
	private:
		std::ofstream F_Stream;
		bool is_Open;
		//for drawing
		std::vector<std::string> Log_History;
	};

	struct A_Application
	{							 
		A_Application();
		~A_Application();

		A_Window Window;
		S_GL_Interfaces::S_GL_Interface OpenGL;
		A_Input Input;
		//event handler	
		//logs
		A_Log Main_Log;			
		A_Log Event_Log;//key presses
		A_Log Error_Log;//oh no					
		A_Log Load_Log;//anything with loading
		A_Log Key_Log;//key presses
		//functions
		void Initialize(HINSTANCE hInstance);
		void Shutdown();

		void Update(float dt);
		void Update_Time();	 
		struct tm* Time;
		time_t Temp_Time;

		bool Has_Shutdown;//is done
	};
}

#endif