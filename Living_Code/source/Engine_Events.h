#ifndef ENGINE_EVENTS
#define ENGINE_EVENTS

#include <Windows.h>
#include <vector>
#include <string>

#include "Core.h"

namespace E_Events
{
	struct Event_Base
	{
		Event_Base();
		Event_Base(u_char ID);					
		virtual std::string To_String();

		u_char E_Code;//code id of event
	};

	struct Event_Default:public Event_Base
	{
		Event_Default();
		Event_Default(u_char ID,u_char Arg);
		std::string To_String();

		u_char Argument;
	};

	struct Event_Win32:public Event_Base
	{
		Event_Win32();
		Event_Win32(u_int Msg, WPARAM wParam, LPARAM lParam);	  
		std::string To_String();

		u_int m_Msg;
		WPARAM m_wParam;
		LPARAM m_lParam;
	};


	struct E_Event_Manager
	{
		E_Event_Manager();
		~E_Event_Manager();


		std::vector<Event_Base*>Event_List;

		void Update();
		void Add_Event(Event_Base* Event);
		void Remove_Event(u_int Pos);
	};






	enum 
	{
		EVENT_NONE = 0,
																											
		EVENT_EXIT,
		
		EVENT_COMMAND,
		EVENT_WIN32,
		EVENT_OPENGL,

		EVENT_KEYDOWN,
		EVENT_KEYUP,

		EVENT_GAME,
		EVENT_SCREEN
	};
}


#endif