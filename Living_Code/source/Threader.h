#ifndef ENGINE_THREADER
#define ENGINE_THREADER

#include <Windows.h>
#include <vector>

#include "Timer.h"
#include "Core.h"

namespace E_Threads
{
	DWORD WINAPI Draw_Thread(void*);
	DWORD WINAPI Logic_Thread(void*);
	//DWORD WINAPI Event_Thread(void*);
	//DWORD WINAPI Resource_Load_Thread(void*); 
	DWORD WINAPI Input_Thread(void*);

#ifdef USE_XBOX
	DWORD WINAPI XBox_Thread(void*);
#endif

	enum {Event_Resize};
	struct T_Event//events for the thread to proccess
	{
		T_Event(unsigned int nID);
		T_Event(unsigned int nID,unsigned int* nValues);
		void Destroy();
		unsigned int ID;
		unsigned int* Values;
	};

	struct T_Thread
	{
		T_Thread();
		~T_Thread();

		void End();
		void Pause();
		void Resume();

		HANDLE hThread;
		DWORD t_ID;//thread id
		bool Running;
		Timer m_Timer;
		std::vector<T_Event> Event_Stack;
		void Add_Event(unsigned int ID);
		void Add_Event(unsigned int ID,unsigned int* Values);
	};
}
	//


/*struct T_Threader
{
	unsigned int Ping;
	
	void Start(void(S_Engine::*func)());
	void End();
private:
	bool Running;
	HANDLE hThread;
	DWORD WINAPI m_Thread(void*);
	void(S_Engine::*m_Func)();
};
	   */
#endif