#include "Functions.h"
#include <XInput.h>
/*#include "App.h"
extern myApp APP;
#include "GameStats.h"
extern GameStats Game;
#include "ServerStats.h"
extern ServerStats Server;
#include "ScreenClass.h"
extern Screen MainScreen;
#include "ModelHolder.h"
#include "ImageHolder.h"
//extern ImageHolder Images;
#include "SoundHolder.h"
extern SoundHolder Sounds;

#include "myMath.h"		   

#define MY_NET_MESSAGE 1224


using namespace UtilityIO;
using namespace ModelHolder;*/

#include "Engine.h"
extern S_Engine Engine;

#include "myMath.h"	 

#define Down_Before (lParam & (1 << 30))

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*if(uMsg == WM_KEYDOWN)
		Engine.Event_Manager.Add_Event(new E_Events::Event_Default(E_Events::EVENT_KEYDOWN, wParam));
	else if(uMsg == WM_KEYUP)  
		Engine.Event_Manager.Add_Event(new E_Events::Event_Default(E_Events::EVENT_KEYUP, wParam));
	else if(uMsg == WM_COMMAND)
		Engine.Event_Manager.Add_Event(new E_Events::Event_Default(E_Events::EVENT_COMMAND, LOWORD(wParam)));
	else
		Engine.Event_Manager.Add_Event(new E_Events::Event_Win32(uMsg,wParam,lParam));
	*/
    
	switch (uMsg)
	{
	    case WM_DESTROY:case WM_CLOSE:case WM_QUIT:
		{
			std::stringstream Get_Msg;Get_Msg<<uMsg;
			Engine.Application.Main_Log.Write_Log("Shutdown by window message: "+Get_Msg.str() );
			Engine.Shutdown();
			return 0;
		}break;  	  
		case WM_COMMAND:
			{
				Engine.Wind_Command(LOWORD(wParam));
			}break;

        /*case WM_ACTIVATE:
        {
            if (!HIWORD(wParam))                    // Check Minimization State
            {
                //active=TRUE;                    // Program Is Active
            }
            else
            {
                //active=FALSE;                   // Program Is No Longer Active
            }
        }break;*/

        case WM_SIZE:
			{
				//      Retrieve width and height
				unsigned int Width=LOWORD(lParam);
				unsigned int Hieght=HIWORD(lParam);

				Engine.Application.Window.Reshape(Width,Hieght);

				//Engine.Thread_Draw.Add_Event(E_Threads::Event_Resize,nSize);
			}break;
		case WM_MOVE:
			{
				//      Retrieve width and height
				unsigned int nX=LOWORD(lParam);
				unsigned int nY=HIWORD(lParam);

				Engine.Application.Window.Move(nX,nY);
			}break;


						  
		//mouse input
		case WM_MOUSEWHEEL:
			{
				short Wheel_Dist=GET_WHEEL_DELTA_WPARAM(wParam);
				Engine.Application.Input.Mouse_Scroll += RoundToUnit(Wheel_Dist);
			}break;
		case WM_MOUSEMOVE:
			{
				Engine.Application.Input.Mouse_Move(LOWORD(lParam),HIWORD(lParam));
			}break;	
		case WM_LBUTTONDOWN:
			{
				if(!Down_Before)Engine.Application.Input.Mouse_L_Down=true;
			}break;
		case WM_LBUTTONUP:
			{
				if(!Down_Before)Engine.Application.Input.Mouse_L_Down=false;
			}break;
		case WM_RBUTTONDOWN:
			{
				if(!Down_Before)Engine.Application.Input.Mouse_R_Down=true;
			}break;
		case WM_RBUTTONUP:
			{
				if(!Down_Before)Engine.Application.Input.Mouse_R_Down=false;
			}break;


		/*case WM_KEYDOWN:
			{
				if(!KeyRep)
				{
					switch(wParam)
					{
					case VK_ESCAPE:
						{
							Engine.Application.Main_Log.Write("Shutdown by escape\n");
							Engine.Shutdown();
						}break;
					}
				}
			}break;			*/	 

		case WM_KEYUP:
			{
				if(Down_Before)// !Engine.Application.Input.XBOX.InUse && 
					Engine.Application.Input.Keys.Update(wParam,false);	
			}break;
		case WM_KEYDOWN:
			{
				if(!Down_Before)
					Engine.Application.Input.Keys.Update(wParam,true);	   
			}break;
	}

	
					
    /*if(APP.Typing && !KeyRep)
    {
        std::string Report="Typing Key Pressed: ";
        if(uMsg==WM_CHAR)
        {
			if(wParam>=33 && wParam<=126 && wParam!=' ' && APP.TypingFor!="commandsSTARTED")
            {
                APP.whatTyping+=(char)wParam;
                Report+=(char)wParam;
            }
			else if(APP.TypingFor=="commandsSTARTED")
			{
				APP.TypingFor="commands";
			}
        }
        else if(uMsg==WM_KEYDOWN)
        {
            switch(wParam)
            {
                case VK_RETURN:
                {
                    std::string newWhat=APP.whatTyping;
                    bool FinnishedTyping=true;
                    std::string LastTypedFor=APP.TypingFor;
                    if(APP.TypingFor=="chat"){}//Chatty.ChatStuff(newWhat.c_str(),"You");}
                    else if(APP.ALLOWCOMMANDS && APP.TypingFor=="commands"){APP.Log.Write(Command(newWhat).c_str());}
					else if(APP.TypingFor=="client/ip"){Server.IPConnectedTo=APP.whatTyping;FinnishedTyping=false;APP.TypingFor="client/port";}//std::stringstream nPort;nPort<<Server.DefaultPort;APP.whatTyping=nPort.str();}
                    else if(APP.TypingFor=="client/port")
                    {
                        Server.PortConnectedTo=atoi(APP.whatTyping.c_str());
                        Server.Connect(Server.IPConnectedTo,Server.PortConnectedTo);
                        APP.ServerLog.Write(("Connecting To: "+Server.IPConnectedTo+":"+APP.whatTyping).c_str());
                    }
                    else if(APP.TypingFor=="host/port")
                    {
                        Server.PortConnectedTo=atoi(APP.whatTyping.c_str());
                        Server.Listen(Server.PortConnectedTo);
                        APP.ServerLog.Write(("Listening To: "+APP.whatTyping).c_str());
                    }
                    else{APP.Log.Write("Could Not Find Typed Handler\n");}
                    if(APP.DEBUG)
                    {
                        APP.Log.Write(("Typed And Entered: "+APP.whatTyping+", For: "+LastTypedFor).c_str());
                    }
                    if(FinnishedTyping)
                    {
                        APP.FinnishedTyping();
                    }
                    else
                    {
                        APP.whatTyping="";
                    }
                    Report+="Enter";
                }break;
                case VK_BACK:
                {
                    if(APP.whatTyping.length()>0)
                    {
                        APP.whatTyping.erase(APP.whatTyping.length()-1);
                        Report+="Backspace";
                    }
                }break;
                case VK_SPACE:
                {
                    APP.whatTyping+=' ';
                    Report+="Space";
                }break;
            }
        }

        Report+="\n";
        if(Report.length()>=22)
        {
            APP.KeyLog.Write(Report.c_str());
        }
    }
	if(SendBack){return DefWindowProc(hWnd, uMsg, wParam, lParam);}
	else
	{*/
		//return 0;
	//}
	
	//if(Return_Back)
	//{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	//}
}