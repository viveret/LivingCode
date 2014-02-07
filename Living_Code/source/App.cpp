#include "App.h"
using namespace E_Application; 
#pragma warning(disable:4996)//disables "unsafe" (deprecated) function warnings.


#include "GL\glew.h"
#include "GL\GL.h" 

#include "Engine.h"
extern S_Engine Engine;
#include "Functions.h"
	  
#include <CommCtrl.h>


A_Window::A_Window()
{
	this->Is_Active=false;//not active yet

	this->Wind_w=1;this->Wind_h=1;
	this->Wind_x=0;this->Wind_y=0;

	this->Fullscreen=false;this->Focus=false;
																						   
	this->hEng_RC=NULL;//  rendering context, links opengl calls to engine
	this->hDraw_RC=NULL;// and draw threads

	this->hDC=NULL;//    device context         draw to the window
	this->hWnd=NULL;//  window handle
	this->hInstance=NULL;//               instance of application
}

A_Window::~A_Window()
{
	this->Destory_Window();
}

void A_Window::Create_Window(HINSTANCE nhInstance)
{		
	if(this->Is_Active) return;//if already active, dont make another

	this->Is_Active=true;

	InitCommonControls();

	this->hInstance=nhInstance;

	this->Wind_w=GetSystemMetrics(SM_CXSCREEN);
	this->Wind_h=GetSystemMetrics(SM_CYSCREEN);

	GLuint		PixelFormat;			// Holds Results
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;

	WindowRect.left=(long)0;
	WindowRect.right=(long)this->Wind_w;
	WindowRect.top=(long)0;
	WindowRect.bottom=(long)this->Wind_h;

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) MainWindowProc;
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= this->hInstance;
	wc.hIcon			= LoadIcon(this->hInstance,IDC_ICON);
	wc.hCursor			= LoadCursorFromFile("stuff/screen/cursor.cur");
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= APP_NAME;

	if (!RegisterClass(&wc))
	{			
		Engine.Win32_Assert( "Failed To Register The Window Class", 3 );
		return;
	}

	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle=WS_OVERLAPPEDWINDOW | WS_THICKFRAME;

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);



    // Create The Window
	if (!(this->hWnd=CreateWindowEx(dwExStyle,
								APP_NAME,							// Class Name
								APP_NAME,								// Window Title
								dwStyle |
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								CW_USEDEFAULT, 0,
								WindowRect.right-WindowRect.left,
								WindowRect.bottom-WindowRect.top,
								NULL,								// No Parent Window
								NULL,								// No Menu
								this->hInstance,
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		Engine.Win32_Assert( "Window Creation Error", 3 );
		return;
	}

	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,	
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		0,											// 0Bit Z-Buffer (Depth Buffer)
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(this->hDC=GetDC(this->hWnd)))// Did We Get A Device Context?
	{
		Engine.Win32_Assert( "Can't Create A GL Device Context", 3 );return;
	}

	if (!(PixelFormat=ChoosePixelFormat(this->hDC,&pfd)))// Did Windows Find A Matching Pixel Format?
	{
		Engine.Win32_Assert( "Can't Find A Suitable PixelFormat", 3 );return;
	}

	if(!SetPixelFormat(this->hDC,PixelFormat,&pfd))// Are We Able To Set The Pixel Format?
	{
		Engine.Win32_Assert( "Can't Set The PixelFormat", 3 );return;
	}

	if (!(this->hEng_RC=wglCreateContext(this->hDC)))// Are We Able To Get A Rendering Context?
	{
		Engine.Win32_Assert( "Can't Create A GL Rendering Context For The Engine", 3 );return;
	}
	
	if (!(this->hDraw_RC=wglCreateContext(this->hDC)))// Are We Able To Get A Rendering Context?
	{
		Engine.Win32_Assert( "Can't Create A GL Rendering Context For The Draw Thread", 3 );return;
	}

	//time to share
	BOOL Share_Error = wglShareLists(this->hEng_RC,this->hDraw_RC);
	if(Share_Error==FALSE)
	{
		Engine.Win32_Assert( "Can't Share Engine and Draw Thread Lists", 3 );return;
	}

	if(!wglMakeCurrent(this->hDC,this->hDraw_RC))// Try To Activate The Rendering Context
	{
		Engine.Win32_Assert( "Can't Activate The GL Rendering Context For Engine", 3 );return;
	}

	if( Engine.Application.OpenGL.Initialize() == false )//failed to initialize
	{
		Engine.Register_Error(S_Error("Failed to initialize OpenGL interface", 3));
		return;
	}


	ShowWindow(this->hWnd,SW_SHOWMAXIMIZED);						// Show The Window
	SetForegroundWindow(this->hWnd);						// Slightly Higher Priority
	SetFocus(this->hWnd);									// Sets Keyboard Focus To The Window

	this->Reshape(this->Wind_w,this->Wind_h);

	/*    OPENGL SETTINGS    */	  

    sglEnable(GL_DEPTH_TEST);
	sglEnable(GL_COLOR_MATERIAL);   
	sglEnable(GL_BLEND);					 
	sglEnable(GL_ALPHA_TEST);	   


    glShadeModel(GL_SMOOTH);
    glClearDepth(1.f);	 
    glClearColor(0.f,0.f,0.f,0.f); 
	glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);		
    glAlphaFunc(GL_GREATER, 0.1f); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glLineWidth( 3 );
	sglEnable( GL_PROGRAM_POINT_SIZE );
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	 
    glClear(GL_DEPTH_BUFFER_BIT);  

	sglEnable(GL_TEXTURE_2D);   
	sglDisable(GL_LIGHTING);
	Engine.Application.OpenGL.GL_Shader->Change(GL_LIGHTING,Vector3(10,10,0));

	float bluish[] = { 1.f, 1.f, 1.f, 1 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, bluish); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, bluish);
	glLightfv(GL_LIGHT0, GL_AMBIENT, bluish);

	sglDisable(GL_FOG);	 


	/*glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f );	
	glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION , 0.0f );	
	glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION , 0.0002 );		  */

	//made it here so far, no errors
}

void A_Window::Destory_Window()
{	 
	if(this->Is_Active==false) return;//already nonactive

	this->Is_Active=false;

	if(this->hEng_RC || this->hDraw_RC)
	{
		/*if (!wglMakeCurrent(NULL,NULL))//Able to release contexts?
		{	
			if(!Engine.Win32_Error(S_Error("Release Of DC And RC Failed",3)))return;
		} */
									 
		if (!wglDeleteContext(this->hEng_RC))//Able to delete?
		{
			Engine.Win32_Assert( "Release Rendering Context For Engine Failed", 3 );return;
		}
		this->hEng_RC=NULL;	
		
		if (!wglDeleteContext(this->hDraw_RC))//Able to delete?
		{
			Engine.Win32_Assert( "Release Rendering Context For Drawing Failed", 3);return;
		}
		this->hDraw_RC=NULL;	
	}

	if (this->hDC && !ReleaseDC(this->hWnd,this->hDC))//Release DC?
	{					
		this->hDC=NULL;
		Engine.Win32_Assert( "Release Device Context Failed", 3);return;
	}

	if (this->hWnd && !DestroyWindow(this->hWnd))//Destroy window?
	{				
		this->hWnd=NULL;
		Engine.Win32_Assert( "Could Not Release hWnd", 3 );return;
	}

	if (!UnregisterClass(APP_NAME,this->hInstance))//Unregister class?
	{			
		this->hInstance=NULL;
		Engine.Win32_Assert( "Could Not Unregister Class", 3 );return;
	}

	//PostQuitMessage(0);
	//finished all the way
}

void A_Window::Reshape(unsigned int w,unsigned int h)
{
	if(w==0){w=1;}
	if(h==0){h=1;}

	this->Wind_w=w;
	this->Wind_h=h;		  
	glViewport(0,0,this->Wind_w,this->Wind_h);					   
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0.0,Engine.Application.Window.Wind_w, 0.0, Engine.Application.Window.Wind_h, -1, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void A_Window::Move(int x,int y)
{
	this->Wind_x=x;
	this->Wind_y=y;
}	

	   


Key::Key()
{
	this->Key_Code=0;
	this->Down=false;
}

A_Input_Keys::A_Input_Keys(std::string File)//load
{
	this->Load_Keymap(File);
}

A_Input_Keys::A_Input_Keys()//defaults
{
	this->Defaults();
}

void A_Input_Keys::Load_Keymap(std::string File)
{
	//
}

void A_Input_Keys::Defaults()
{
	this->Quit.Key_Code=VK_ESCAPE;//quit the application
	this->Commands.Key_Code=VK_OEM_3;// ~
	//MENU
	this->Pause.Key_Code=VK_TAB;
	this->To_Backpack.Key_Code='Z';
	this->To_Map.Key_Code='M';

	//GAMEPLAY
	this->Forward.Key_Code='W';
	this->Backward.Key_Code='S';
	this->Left.Key_Code='A';
	this->Right.Key_Code='D';

	this->Eat.Key_Code = 'E';
	this->Toggle_Think.Key_Code = 'Q';

	this->Jump.Key_Code=VK_SPACE;
	this->Crouch.Key_Code='Q';
	this->Run.Key_Code=VK_SHIFT;

	this->Zoom.Key_Code='C';

	this->Primary_Fire.Key_Code=VK_LBUTTON;
	this->Secondary_Fire.Key_Code=VK_RBUTTON;

	this->Drop_Item.Key_Code=VK_BACK;
	this->Reload.Key_Code='R';
	this->Chat.Key_Code=VK_RETURN;
}	

void A_Input_Keys::Update(u_int Key_Code,bool Set)
{
	if(Key_Code==this->Quit.Key_Code)
		this->Quit.Down=Set;
	else if(Key_Code==this->Commands.Key_Code)	 
		this->Commands.Down=Set;
		//MENU
	else if(Key_Code==this->Pause.Key_Code)	  
		this->Pause.Down=Set;
	else if(Key_Code==this->To_Backpack.Key_Code)	 
		this->To_Backpack.Down=Set;
	else if(Key_Code==this->To_Map.Key_Code)	  
		this->To_Map.Down=Set;

		//GAMEPLAY
	else if(Key_Code==this->Forward.Key_Code)	 
		this->Forward.Down=Set;
	else if(Key_Code==this->Backward.Key_Code)	   
		this->Backward.Down=Set;
	else if(Key_Code==this->Left.Key_Code)		 
		this->Left.Down=Set;		
	else if(Key_Code==this->Right.Key_Code)		 
		this->Right.Down=Set;
									 
	else if( Key_Code == this->Eat.Key_Code )		 
		this->Eat.Down = Set;
	else if( Key_Code == this->Toggle_Think.Key_Code )		 
		this->Toggle_Think.Down = Set;

	else if(Key_Code==this->Jump.Key_Code)		  
		this->Jump.Down=Set;
	else if(Key_Code==this->Crouch.Key_Code)		  
		this->Crouch.Down=Set;
	else if(Key_Code==this->Run.Key_Code)		 
		this->Run.Down=Set;
	else if(Key_Code==this->Zoom.Key_Code)		
		this->Zoom.Down=Set;

	else if(Key_Code==this->Primary_Fire.Key_Code)			
		this->Primary_Fire.Down=Set;
	else if(Key_Code==this->Secondary_Fire.Key_Code)//shooting	
		this->Secondary_Fire.Down=Set;
	else if(Key_Code==this->Drop_Item.Key_Code)		
		this->Drop_Item.Down=Set;
	else if(Key_Code==this->Reload.Key_Code)	  
		this->Reload.Down=Set;
		
	else if(Key_Code==this->Chat.Key_Code)		 
		this->Chat.Down=Set;
}


Raw_Buffer::Raw_Buffer()
{
	this->my_Buf = 0;
	this->Length = 0;
}

void Raw_Buffer::Delete()
{
	if( this->my_Buf )
	{
		delete[] this->my_Buf;
		this->my_Buf = NULL;

		this->Length = 0;
	}
}

A_Input::A_Input()
{
	this->Typing=false;//is typing
		
	XBOXController XBOX;
	A_Input_Keys Keys;

	this->Mouse_X=0;this->Mouse_Y=0;
	this->Mouse_Scroll=0;
	this->Mouse_L_Down=false;this->Mouse_R_Down=false;//menu
}

A_Input::~A_Input()
{
	//
}

std::string A_Input::Load_File_Str( std::string File, bool Silent)//load raw file, can be silent = no log
{
	Raw_Buffer Temp_Buf = this->Load_File( File, Silent );

	if( Temp_Buf.my_Buf != NULL )
	{
		std::string Ret( Temp_Buf.my_Buf );
		Temp_Buf.Delete();
		return Ret;
	}
	else 
		return std::string();
}

Raw_Buffer A_Input::Load_File( std::string File, bool Silent)//load raw file, can be silent = no log
{
	if( Silent == false )
	{
		Engine.Application.Load_Log.Print_Time();
		Engine.Application.Load_Log.Write( "\tLoading File: " + File + "... ");
	}

	std::ifstream Reader(File,std::ios::binary);	 

	Raw_Buffer Ret;

	if(Reader.good())
	{		 
		Reader.seekg(0,std::ios::end);
		Ret.Length=(unsigned int)Reader.tellg();

		Ret.my_Buf = new char[Ret.Length+1]();

		Reader.seekg(0,std::ios::beg);

		Reader.read(Ret.my_Buf,Ret.Length);

		if( Silent == false )
			Engine.Application.Load_Log.Write("Success, Size: " + To_Str(Ret.Length) + "\n\n");
	}
	else
	{
		std::string Report = "Failed to load file: " + File;
		Engine.Register_Error(S_Error(Report,1));

		if( Silent == false )
		Engine.Application.Load_Log.Write("Failed\n\n");
	}		  

	return Ret;
}

void A_Input::Mouse_Move(unsigned int x,unsigned int y)
{
	this->Mouse_X = x;
	this->Mouse_Y = Engine.Application.Window.Wind_h - y - 224;
}

A_Log::A_Log()
{
	this->is_Open=false;

	this->Record_History = true;
}

A_Log::~A_Log()
{
	if(this->is_Open)
	{
		this->Write_Log("Was Not Explicitly Closed.");
		this->Close();
	}
}

bool A_Log::Open(std::string Where)
{
	if(!this->is_Open)//if not already open
	{
		this->F_Stream.open(Where.c_str());
		if(this->F_Stream.is_open())
		{
			this->is_Open=true;
			this->Write_Log("Opened Successfully.");
			return true;
		}
	}
	return false;
}		   		

void A_Log::Write(std::string What)
{		  
	if(this->is_Open && Engine.Application.Time)
	{										
		this->F_Stream << What;
		this->F_Stream.flush();

		if(this->Record_History)
			this->Log_History.push_back( What );
	}
}

void A_Log::Write_Line(std::string What)
{
	if(this->is_Open && Engine.Application.Time)
	{										
		std::string Writer = "\t";

		for(u_int l = 0;l < What.length(); l++)
			if( What[l] == '\n' )
				Writer += "\n\t";
			else
				Writer += What[l];

		Writer += "\n";

		this->F_Stream << Writer;
		this->F_Stream.flush();

		if(this->Record_History)
			this->Log_History.push_back( Writer );
	}
}

void A_Log::Write_Log(std::string What)
{
	this->Print_Time();
	this->Write_Line(What);
	this->Write("\n");	 
}

void A_Log::Print_Time()
{		
	if(this->is_Open && Engine.Application.Time)
	{			
		Engine.Application.Update_Time();

		std::stringstream Writer;
		Writer << ">" << Engine.Application.Time->tm_hour << ':' << Engine.Application.Time->tm_min << ':' << Engine.Application.Time->tm_sec << "--\n";
													
		this->F_Stream << Writer.str();
		this->F_Stream.flush();	
	}
}

void A_Log::Close()
{
	if(this->is_Open)
	{
		this->Write_Log("Closing...");
		this->F_Stream.close();
		this->is_Open=false;

		this->Clear_History();
	}
}

void A_Log::Clear_History()
{
	this->Log_History.clear();
}

A_Application::A_Application()
{ 
	this->Has_Shutdown = false;

	time(&this->Temp_Time);
	this->Time=localtime(&this->Temp_Time);

	this->Error_Log.Open("stuff/data/error_log.txt");

	if(!this->Event_Log.Open("stuff/data/event_log.txt"))
		this->Error_Log.Write_Log("Could Not Event Main Log");
	
	if(!this->Main_Log.Open("stuff/data/main_log.txt"))
		this->Error_Log.Write_Log("Could Not Make Main Log");
	
	if(!this->Load_Log.Open("stuff/data/load_log.txt"))
		this->Error_Log.Write_Log("Could Not Make Load Log");
																						   
	if(!this->Key_Log.Open("stuff/data/key_log.txt"))
		this->Error_Log.Write_Log("Could Not Make Key Log");


	if(CoInitialize(NULL)==S_FALSE)
	{
		Engine.Register_Error(S_Error("Com Initialize",3));
		return;
	}
}	

A_Application::~A_Application()
{
	if(!this->Has_Shutdown)
		this->Shutdown();
}

void A_Application::Initialize(HINSTANCE hInstance)
{				
	this->Window.Create_Window(hInstance);
	srand( time(NULL) );
}

void A_Application::Shutdown()
{			   
	this->Has_Shutdown = true;

	this->OpenGL.Destroy();

	this->Window.Destory_Window();

	this->Main_Log.Close();
	this->Load_Log.Close();	 
	this->Key_Log.Close();	 
	this->Event_Log.Close();
	this->Error_Log.Close();
}

void A_Application::Update(float dt)
{										  
	this->Update_Time();

	this->Input.XBOX.Do( dt );




	//Key Presses

	/*if(this->Input.Pause && !this->Input.Typing)
    {
		if(Engine.In_Game)
			Engine.Pause_Game();
		else if(Engine.Screen.Menu_Trace[Engine.Screen.Menu_Trace.size()]=="pause")
			Engine.Resume_Game();
        //APP.KeyLog.Write("Paused Game\n");
    }*/

	if(this->Input.Keys.Quit.Down)
	{
		this->Input.Keys.Quit.Down=false;
		Engine.Application.Main_Log.Write_Log("Shutdown By Escape");
		Engine.Shutdown();
	}
	else if(this->Input.Keys.Pause.Down && !this->Input.Typing)
	{
		this->Input.Keys.Pause.Down=false;
		if(Engine.In_Game)
			Engine.Pause_Game();
		else
			Engine.Resume_Game();
			//else if(APP.Playing==false && MainScreen.Screen_Depth[MainScreen.Screen_Depth.size()]=="pause"){APP.Resume();}
			//APP.KeyLog.Write("Paused Game\n");
	}
}

void A_Application::Update_Time()
{
    time(&this->Temp_Time);
	localtime(&this->Temp_Time);
}