#include "Engine_Events.h"
using namespace E_Events;
#pragma warning(disable:4996)


Event_Base::Event_Base()
{
	this->E_Code = 0;
}

Event_Base::Event_Base(u_char ID)
{
	this->E_Code = ID;
}

std::string Event_Base::To_String()
{
	char ID[3];itoa(this->E_Code,ID,10);
	std::string Ret="Base Event - "+std::string(ID)+", ";
	switch(this->E_Code)
	{
	case EVENT_NONE:Ret+="None";break;
	case EVENT_EXIT:Ret+="Exit";break;
	default:Ret+="N/A";break;
	}
	return Ret;
}

Event_Default::Event_Default()
{
	this->Argument = 0;
}

Event_Default::Event_Default(u_char ID,u_char Arg)
{
	this->E_Code = ID;
	this->Argument = Arg;
}

std::string Event_Default::To_String()
{				  		   
	char ID[4];memset(ID,0,4);itoa(this->E_Code,ID,10);
	char Arg[12];memset(Arg,0,12);sprintf(Arg,"%u",this->Argument);
	std::string Ret="Default Event - "+std::string(ID)+", "+std::string(Arg)+", ";
	switch(this->Argument)
	{
	case EVENT_NONE:Ret+="None";break;
	case EVENT_EXIT:Ret+="Exit";break;
	default:Ret+="N/A";break;
	}
	return Ret;
}

Event_Win32::Event_Win32()
{					 
	this->m_Msg = 0;
	this->m_wParam = 0;
	this->m_lParam = 0;
}

Event_Win32::Event_Win32(u_int Msg, WPARAM wParam, LPARAM lParam)
{
	this->m_Msg = Msg;
	this->m_wParam = wParam;
	this->m_lParam = lParam;
}


std::string Event_Win32::To_String()
{				    		   
	char ID[4];memset(ID,0,4);itoa(this->E_Code,ID,10);
	char Arg[12];memset(Arg,0,12);sprintf(Arg,"%u",this->m_Msg);
	std::string Ret="Win32 Event - "+std::string(ID)+", "+std::string(Arg)+", ";
	memset(Arg,0,12);sprintf(Arg,"%u",this->m_wParam);Ret+=std::string(Arg)+", ";
	memset(Arg,0,12);sprintf(Arg,"%u",this->m_lParam);Ret+=std::string(Arg)+", ";

	switch(this->m_Msg)
	{
	case EVENT_NONE:Ret+="None";break;
	case EVENT_EXIT:Ret+="Exit";break;
	default:Ret+="N/A";break;
	}
	return Ret;
}



#include "Engine.h"
extern S_Engine Engine;


E_Event_Manager::E_Event_Manager()
{
	//
}

E_Event_Manager::~E_Event_Manager()
{
	if( this->Event_List.size() > 0 )
	{
		Engine.Application.Event_Log.Write_Log("Unread Events:");
		while(this->Event_List.size()>0)
		{
			Engine.Application.Event_Log.Write("\t"+this->Event_List[this->Event_List.size()-1]->To_String());
			this->Remove_Event(0);
		}
	}
}

void E_Event_Manager::Update()
{
	while(this->Event_List.size()>0)
	{
		switch(this->Event_List[0]->E_Code)
		{
		case EVENT_NONE:break;//none		
		
		case EVENT_COMMAND:	
			{
				//
			}break;
		case EVENT_WIN32: 
			{
				//
			}break;
		case EVENT_OPENGL:
			{
				//
			}break;

		case EVENT_KEYDOWN:
			{
				//
			}break;
		case EVENT_KEYUP:  
			{
				//
			}break;

		case EVENT_GAME:  
			{
				//
			}break;
		case EVENT_SCREEN: 
			{
				//
			}break;			 

		case EVENT_EXIT:
			{
				//
			}break;
		}
		this->Remove_Event(0);
	}
}

void E_Event_Manager::Add_Event(Event_Base* Event)
{
	this->Event_List.push_back(Event);
	Engine.Application.Event_Log.Write_Log("Event Added: "+Event->To_String());
}

void E_Event_Manager::Remove_Event(u_int Pos)
{
	if(Pos < this->Event_List.size())
	{																	  
		Engine.Application.Event_Log.Write_Log("Event Removed: "+this->Event_List[Pos]->To_String());
		delete this->Event_List[Pos];										
		this->Event_List.erase(this->Event_List.begin()+Pos);
	}
}