#include "Threader.h"
using namespace E_Threads;

#include "Engine.h"
extern S_Engine Engine;

DWORD WINAPI E_Threads::Draw_Thread(void*)
{
	Engine.Thread_Draw.m_Timer.Start(); 
	Engine.Thread_Draw.t_ID=GetCurrentThreadId();	  						 

	wglMakeCurrent(Engine.Application.Window.hDC,Engine.Application.Window.hDraw_RC);

	Engine.Thread_Draw.Running=true;
	while(Engine.Is_Running)
	{							  
		Engine.Draw();	   					
		Engine.Thread_Draw.m_Timer.Update();
	}  
	
	wglMakeCurrent(NULL,NULL);

	Engine.Thread_Draw.m_Timer.End();
	Engine.Thread_Draw.Running=false;
    return (DWORD) 0;
}	

DWORD WINAPI E_Threads::Logic_Thread(void*)
{
	Engine.Thread_Logic.m_Timer.Start(); 
	Engine.Thread_Logic.t_ID=GetCurrentThreadId();	  						  
	AttachThreadInput(Engine.Thread_Logic.t_ID,Engine.Thread_Engine,TRUE);
	AttachThreadInput(Engine.Thread_Engine,Engine.Thread_Logic.t_ID,TRUE);

	Engine.Thread_Logic.Running=true;
	while(Engine.Is_Running)
	{							  
		Engine.Logic();	   					   
		Engine.Thread_Logic.m_Timer.Update();
	}  
	Engine.Thread_Logic.m_Timer.End();
	Engine.Thread_Logic.Running=false;
    return (DWORD) 0;
}

/*DWORD WINAPI E_Threads::Event_Thread(void*)
{
	Engine.Thread_Events.Running=true;
	while(Engine.Is_Running())
	{
		//Engine.E		   
	}	 
	Engine.Thread_Events.Running=false;
    ExitThread(0);
}

DWORD WINAPI E_Threads::Resource_Load_Thread(void*)
{					
	//get loading context working
	Engine.Resources.Stuff.hRC=wglCreateContext(Engine.Application.Window.hDC);//made
	wglShareLists(Engine.Application.Window.hRC,Engine.Resources.Stuff.hRC);//shared		   
	//GET LOADING CONTEXT
	wglMakeCurrent(Engine.Application.Window.hDC,Engine.Resources.Stuff.hRC);
	//ok to start loading
	Engine.Resources.Initialize();
//	Engine.Resources.Load_World_Resources_Thread(
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(Engine.Resources.Stuff.hRC);
    ExitThread(0);
}	*/

#ifdef USE_XBOX
DWORD WINAPI E_Threads::XBox_Thread(void*)
{
	Engine.Thread_XBox.m_Timer.Start();	
	Engine.Thread_XBox.t_ID=GetCurrentThreadId();
	while(Engine.Is_Running)
	{							    	
		Engine.Application.Input.XBOX.Check();
		Engine.Application.Input.XBOX.Do(0.f);	
		Engine.Thread_XBox.m_Timer.Update();	 
	}  
	Engine.Thread_XBox.m_Timer.End();
    return (DWORD) 0;
}
#endif

DWORD WINAPI E_Threads::Input_Thread(void*)
{
	Engine.Thread_Input.m_Timer.Start();	
	Engine.Thread_Input.t_ID=GetCurrentThreadId();
	//
	AttachThreadInput(Engine.Thread_Input.t_ID,Engine.Thread_Engine,TRUE);
	//
	while(Engine.Is_Running)
	{							  
		//Engine.Application.Input.Get();
//		GetKeyboardState(Engine.Application.Input.Keys); 
		Engine.Thread_Input.m_Timer.Update();
	}  
	Engine.Thread_Input.m_Timer.End();
    return (DWORD) 0;
}

E_Threads::T_Event::T_Event(unsigned int nID)
{
	this->ID=nID;
	this->Values=NULL;
}

E_Threads::T_Event::T_Event(unsigned int nID,unsigned int* nValues)
{
	this->ID=nID;
	this->Values=nValues;
}

void E_Threads::T_Event::Destroy()
{
	if(this->Values)
	{
		delete[] this->Values;
	}
}

void E_Threads::T_Thread::Add_Event(unsigned int ID)
{
	this->Event_Stack.push_back(E_Threads::T_Event(ID));
}

void E_Threads::T_Thread::Add_Event(unsigned int ID,unsigned int* Values)
{
	this->Event_Stack.push_back(E_Threads::T_Event(ID,Values));
}



T_Thread::T_Thread()
{
	this->hThread=NULL;
	this->Running=true;
	this->t_ID=NULL;
}

T_Thread::~T_Thread()
{
	this->End();
}

void T_Thread::End()
{
	if(this->hThread)
	{
		WaitForSingleObject(this->hThread,INFINITE);
		CloseHandle(this->hThread);
		this->hThread=NULL;
	}
}

void T_Thread::Pause()
{
	//
}

void T_Thread::Resume()
{
	//
}

/*void T_Threader::Start(void(S_Engine::*func)())
{
	if(!this->Running)
	{
		this->m_Func=func;
		this->Running=true;
		this->hThread=CreateThread(NULL,0,&this->m_Thread,(void*)this,0,NULL);
	}
}

void T_Threader::End()
{
	this->Running=false;
}

DWORD WINAPI T_Threader::m_Thread(void* nThis)
{
	T_Threader* This=(nThis);
	while(this->Running)
		{
			this->Ping=0;
			(this->m_Func);
		}
}		  */