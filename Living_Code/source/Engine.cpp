#include "Engine.h"

S_Engine::S_Engine()
{
	this->Is_Running=true;this->In_Game=true;		
}

S_Engine::~S_Engine()
{

}

void S_Engine::Start(HINSTANCE hInstance)
{	
	this->Thread_Engine=GetCurrentThreadId();


	this->Get_Options();
	this->Application.Initialize(hInstance);
	this->Resources.Load_Stuff();

	this->Game.Initialize();


	wglMakeCurrent(NULL,NULL);
	
	// thread caps				 
	this->E_Timer.Frame_Time=1000.f/ENGINE_TPS/1000.f;
	this->Thread_Logic.m_Timer.Frame_Time=1000.f/LOGIC_TPS/1000.f;
	this->Thread_Draw.m_Timer.Frame_Time=1000.f/DRAW_TPS/1000.f;
																									
	this->Thread_Logic.hThread=CreateThread(NULL,0,E_Threads::Logic_Thread,(void*)NULL,0,NULL);	
	this->Thread_Draw.hThread=CreateThread(NULL,0,E_Threads::Draw_Thread,(void*)NULL,0,NULL);
	this->Thread_Input.hThread=CreateThread(NULL,0,E_Threads::Input_Thread,(void*)NULL,0,NULL);

#ifdef USE_XBOX
	this->Thread_XBox.hThread=CreateThread(NULL,0,E_Threads::XBox_Thread,(void*)NULL,0,NULL);
#endif

	//AttachThreadInput(this->Thread_Engine,this->Thread_Logic.t_ID,TRUE);


	//Other initializes
	this->E_Timer.Start();
}

void S_Engine::Shutdown()
{
	if(this->Is_Running)
	{
		this->Is_Running=false; 			
	}
}

void S_Engine::Exit()
{		
	if(this->Is_Running==false)
	{					
		//this->Thread_XBox.End();
		//this->Thread_Input.End();							 
		this->Thread_Draw.End();  									 
		this->Thread_Logic.End();  	


		this->Game.Destroy();	


		this->Application.Shutdown();
	}
}

void S_Engine::Register_Error(S_Error nError)
{
	if(nError.Level>0)//is somewhat important
	{
		std::string Error_Rep;
		switch(nError.Level)
		{
		case 1:Error_Rep="Warning";break;
		case 2:Error_Rep="Minor Error";break;
		case 3:Error_Rep="Major Error";break;
		case 4:Error_Rep="Alert";break;
		case 5:Error_Rep="Debug Message";break;
		}
		Error_Rep+= ": " + nError.Message;
		this->Application.Error_Log.Write_Log(Error_Rep);

		if(nError.Level==3)
		{
			MessageBox(NULL,nError.Message.c_str(),"CRITICAL ERROR",MB_OK | MB_ICONINFORMATION);
			this->Shutdown();
		}
	}
}
  
bool S_Engine::Win32_Assert(std::string Info, u_char Level)// 0 = none, 1 = log, 2 = don't continue, 3 = shutdown
{
	DWORD n_Error = GetLastError();
	LPSTR lpMsgBuf=NULL;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				  FORMAT_MESSAGE_FROM_SYSTEM | 
				  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL,n_Error,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				  (LPTSTR)&lpMsgBuf,0,NULL);
										 
	std::stringstream Str_Err; 
	Str_Err<<n_Error<<'-'<<lpMsgBuf;
	
	Info += ": " + Str_Err.str();

	LocalFree( lpMsgBuf );

	if(n_Error == ERROR_SUCCESS)
	{
		return false;
	}			   
	this->Register_Error(S_Error( Info, Level) );
	return true;
}

#include "Functions.h"

bool S_Engine::OpenGL_Assert(std::string Info, u_char Level)// 0 = none, 1 = log, 2 = don't continue, 3 = shutdown
{
	GLenum n_Error = glGetError();

	if(n_Error==GL_NO_ERROR && Level != 5)
	{
		return false;
	}			
	else
	{			  
		const char* glu_Str = (const char*)gluErrorString(n_Error);
	
		Info += ": " + To_Str(n_Error);

		if(glu_Str)
			Info += " " + std::string(glu_Str);

		this->Register_Error(S_Error(Info, Level) );
		return true;
	}
}

#include <ShlObj.h>

void S_Engine::Wind_Command(unsigned int Com)
{
	switch(Com)
	{
	case 0:
		return;
		break;

	default:
		{
			if(false)
			{
				char Buf[16];
				memset(Buf,0,16);	
				_itoa_s(Com,Buf,10);
				std::string Bad_Com(Buf);
				this->Register_Error(S_Error("Unknown Window Command "+Bad_Com,0));
			}
		}break;
	}
}

void S_Engine::Logic()
{
	float nDT = this->Thread_Logic.m_Timer.dt;// * (abs(this->Application.Input.Mouse_Scroll) * 0.1f);

	this->Application.Update( nDT );
	if( this->In_Game )
	{	
		this->Game.Update( nDT );
	}
}

void S_Engine::Main_Loop()
{				
	this->Event_Manager.Update();


	this->E_Timer.Update();
}
				  
#include "Draw_Utility.h"
using namespace Draw_Utility;
void S_Engine::Make_Game()
{
	this->Game.Initialize();
}

void S_Engine::Enter_Game()
{	
	this->In_Game=true;							   
	this->Application.Main_Log.Write_Log("Entered game");
}

void S_Engine::Enter_Screen()
{						
//	this->Screen.Menu_Trace.clear();
//	this->Screen.Load_Menu("pause");
	this->In_Game=false;
	this->Application.Main_Log.Write_Log("Entered Menu");
}

void S_Engine::Pause_Game()
{

	this->Enter_Screen();
	this->Application.Main_Log.Write_Log("Paused Game");
}

void S_Engine::Resume_Game()
{
	this->Enter_Game();
	this->Application.Main_Log.Write_Log("Resumed Game");
}


void S_Engine::Get_Options()
{
	this->Application.Main_Log.Print_Time();
	this->Application.Main_Log.Write("\tGetting Options... ");
	std::string Data = this->Application.Input.Load_File_Str("stuff/data/options.txt");
	
	if(Data.length()==0)
	{
		this->Register_Error(S_Error("Cannot load options, no data",1));
		return;
	}
	else
		this->Application.Main_Log.Write("Success.\n");

	std::vector<std::string> Options;
	std::vector<std::string> Variables;

	char Buffer[64];memset(Buffer,0,64);
	char Buf_At=0;

	for(u_int l=0;l<Data.size();l++)
	{		
		if( Data[l] == ':' )
		{
			Options.push_back(std::string(Buffer));
			memset(Buffer,0,64);  
			Buf_At = 0;	  
		}
		else if(Data[l]=='\n' && Options.size() -1 == Variables.size())//done with that data. and added option
		{
			Variables.push_back(std::string(Buffer));
			memset(Buffer,0,64);  
			Buf_At = 0;
		}
		else if( (isalnum(Data[l]) || Data[l] == ' ' || Data[l] == '.') && Data[l] != 13 && Buf_At<64 )
		{
			Buffer[Buf_At]=Data[l];
			Buf_At++;
		}
		else if(Buf_At == 64)
		{
			memset(Buffer, 0, 64);
			Buf_At = 0;
		}
	}

	for(u_int i = 0;i < Options.size(); i++)
	{																   
		if(Options[i] == "texture load")
		{
			if(Variables[i] == "fast")
				this->Resources.Load_Fast = true;
		}		
		else if(Options[i] == "record bad opengl change")
		{
			if(Variables[i] == "true")
				this->Application.OpenGL.Record_Bad_Change = true;
		}							
		else if( Options[i] == "force old shaders" )
		{
			if( Variables[i] == "true" )
				this->Application.OpenGL.Use_New_Shaders = false;
		}
		else if( Options[i] == "force old rtt" )
		{
			if( Variables[i] == "true" )
				this->Application.OpenGL.Use_New_RTT = false;
		}
		else if( Options[i] == "force old vbo" )
		{
			if( Variables[i] == "true" )
				this->Application.OpenGL.Use_New_VBO = false;
		}		
		else if( Options[i] == "world width" )
		{
			this->Game.World_W = atoi( Variables[i].c_str() );
		}
		else if( Options[i] == "grass spawn time" )
		{
			this->Game.Grass_Spawn_Time = atof( Variables[i].c_str() );
		}
		else if( Options[i] == "grass max amount" )
		{
			this->Game.Grass_Max_Amount = atoi( Variables[i].c_str() );
		}						   
		else if( Options[i] == "max ais" )
		{
			this->Game.Max_AIs = atoi( Variables[i].c_str() );
		}		   
		else if( Options[i] == "harshness" )
		{
			this->Game.Harshness = (float)atof( Variables[i].c_str() ) / 100.f;
		}
		else
			continue;//do not log	

		this->Application.Main_Log.Write_Line( Options[i] + " = " + Variables[i] );
	}
	this->Application.Main_Log.Write("\n");
}