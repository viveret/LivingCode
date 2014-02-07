/*
	This is the survivor engine.
	It holds and controls everything.
	
	the app is what is needed to run, the core
	
	the game is what you play in, changing alot
	the screen is also something you move around in, is overlayed on the game, if one.

	the resources can be app && screen: never change
	or game content: always changing

*/

#ifndef SURVIVOR_ENGINE
#define SURVIVOR_ENGINE

#include "Engine_Events.h"
#include "App.h"
#include "Game.h"
#include "Lua.h"
#include "Resources.h"
#include "Threader.h"  
#include "Debug.h"
#include "Timer.h"

struct S_Engine
{
	S_Engine();
	~S_Engine();  

	void Start(HINSTANCE hInstance);
	void Shutdown();
	void Exit();//actual destructor
	void Register_Error(S_Error nError);//do error stuff		
	bool Win32_Assert(std::string Info, u_char Level);
	bool OpenGL_Assert(std::string Info, u_char Level);
										
	void Wind_Command(unsigned int Com);

	//void Thread_Handle();

	/*    ENGINE COMPONENTS    */

	E_Events::E_Event_Manager Event_Manager;
	E_Application::A_Application Application;
	E_Game::G_Game Game;

	E_Resources::R_Resources Resources;
	//E_Debug::D_Debug Debug;

	/*   ENGINE THREADS   */
	DWORD Thread_Engine;
	Timer E_Timer;
	std::vector<E_Threads::T_Event> Event_Stack;
	
	E_Threads::T_Thread Thread_Draw;   
	E_Threads::T_Thread Thread_Logic;
	//E_Threads::T_Thread Thread_Events;
	//E_Threads::T_Thread Thread_Resource;
#ifdef USE_XBOX
	E_Threads::T_Thread Thread_XBox;
#endif

	E_Threads::T_Thread Thread_Input;

	/*   DEFAULT ENGINE FUNCTIONS   */
	
	LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Draw();//called by draw thread
	void Draw_Events();//window/opengl stuff

	void Logic();//math, etc, called by logic thread
	void Logic_Events();//input?

	void Main_Loop();//in main

	/*    ENGINE FUNCTIONS    */

	void Make_Game();//make a new-default level to access
	void Enter_Game();//go into game
	void Enter_Screen();//go into menu

	void Pause_Game();//get ready to enter menu
	void Resume_Game();//get ready to enter game

	/*    ENGINE VARIABLES    */

	void Get_Options();


	bool In_Game,Is_Running;
};	


#endif