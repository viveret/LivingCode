/*#include "Screen.h"		 
using namespace E_Screen;

#include "Engine.h"
extern S_Engine Engine;

void S_Screen::Load_Menu(std::string Menu)
{
	this->Clear_Menu();//set to nothing

	bool Go_Back=true;

	if(Menu=="back")
	{																	  
		this->Menu_Trace.pop_back();
		std::string Last_Menu=this->Menu_Trace[this->Menu_Trace.size()-1];
		this->Menu_Trace.pop_back();
		this->Load_Menu(Last_Menu);
		return;
	}
	else if(Menu=="pause")
    {
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.Pause));

		this->Add_Button(Button_Menu(-200,-48,"Resume","resume"));
		this->Add_Button(Button_Menu(-200,0,"Main Menu","title"));
		this->Add_Button(Button_Menu(-200,48,"Edit-Play","edit/play"));
		this->Add_Button(Button_Menu(-200,96,"Options","options"));
		this->Add_Button(Button_Menu(-200,142,"Save & Quit","save/quit"));

		Go_Back=false;
    }
    else if(Menu=="title")
    {					
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.Title));

		this->Add_Button(Button_Menu(-200,-100,"Single Player","play/single"));
		this->Add_Button(Button_Menu(-200,-52,"Online","play/online/choose"));
		this->Add_Button(Button_Menu(-200,-4,"Options","options"));
		this->Add_Button(Button_Menu(-200,44,"Update","update"));
		this->Add_Button(Button_Menu(-200,92,"Quit","quit"));

		Go_Back=false;
    }
    else if(Menu=="end")
    {				   
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.End));

		this->Add_Button(Button_Menu(-200,0,"Restart","end/play"));
		this->Add_Button(Button_Menu(-200,48,"Continue","end/zombie"));
		this->Add_Button(Button_Menu(-200,96,"Main Menu","title"));

		Go_Back=false;
    }
    else if(Menu=="edit/play")
    {
/*        if(APP.ALLOWEDIT)
        {
            Game.AllowEdit=!Game.AllowEdit;
            APP.Log.Write("Toggled Edit Mode\n");
        }
        Screen::LoadScreen("pause");*/	 /*
    }
    else if(Menu=="save/quit")
    {
/*        if(APP.ALLOWEDIT)
        {
            APP.Log.Write(("Saving Map: "+APP.MAPNAME+"...\n").c_str());
            Game.SaveMap.Save(APP.MAPNAME);
            //myMap.Save(APP.MAPNAME,0,0);
        }
        else
        {
            //save game
        }
        Screen::LoadScreen("title");   */	 /*
    }
    else if(Menu=="quit")
    {
		Engine.Application.Main_Log.Write("Shutdown by menu\n");
		Engine.Shutdown();
    }
    else if(Menu=="play/single")
    {			  
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.Play_Game));

		this->Add_Button(Button_Menu(-200,0,"Story","play/story"));
		//this->Add_Button(Button_Menu(-200,48,"Random Map","play/new"));
    }
    else if(Menu=="play/story")
    {	
		Engine.Make_Game();
		Engine.Enter_Game();
//		Game.Init();
//        Game.Map.newRandom();
//        APP.Resume();
//        Game.AllowEdit=false;
        //Game.LoadMap.Load(APP.MAPNAME);
    }
    else if(Menu=="play/new")
    {
//		Game.Init();
//        Game.Map.newRandom();
//        APP.Resume();
    }
    else if(Menu=="play/online/choose")
    {
//		Server.DisConnect();	
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.Online));

		this->Add_Button(Button_Menu(-200,0,"Join","play/online/client"));
		this->Add_Button(Button_Menu(-200,48,"Host","play/online/host"));
    }
    else if(Menu=="play/online/client")
    {					 
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.Online));

//        Server.Host=false;
//        APP.HOST=false;
//        APP.Typing=true;
//        APP.TypingFor="client/ip";
		//APP.MenuTyping=Server.DefaultIP;
    }
    else if(Menu=="play/online/host")
    {					
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.Online));

		this->Add_Button(Button_Menu(-200,144,"Back","play/online/choose"));

//        Server.Host=true;
//        APP.HOST=true;
//        APP.Typing=true;
//        APP.TypingFor="host/port";
		/*std::stringstream nPort;nPort<<Server.DefaultPort;
		APP.MenuTyping=nPort.str();*/		/*
    }
    else if(Menu=="resume")
    {
//        APP.Resume();
    }
    else if(Menu=="options")
    {				   
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.Options));
										 
		this->Add_Button(Button_Menu(-200,-100,"Gameplay","options/gameplay"));
		this->Add_Button(Button_Menu(-200,-52,"Video","options/video"));
		this->Add_Button(Button_Menu(-200,-4,"Audio","options/audio"));	
		this->Add_Button(Button_Menu(-200,44,"Controls","options/controls"));

		//APP.Options.Save_Ops();
    }
	else if(Menu=="options/gameplay")
	{					
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.Options));	

//		this->Add_Button(Button_Var_Bool(-200,86,"Clean",&APP.Options.Clean));
	}
	else if(Menu=="options/video")
	{				
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.Options));		

//		this->Add_Button(Button_Var_Bool(-200,86,"Fullscreen",&APP.Options.Fullscreen));
	}
	else if(Menu=="options/audio")
	{				
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.Options));		
					
//		this->Add_Button(Button_Var_Slider(-200,38,"Master Volume",&APP.Options.Master_Volume));
	}	
	else if(Menu=="options/controls")
	{				  
		this->Screen_Images.push_back(S_Image(0,-100,640,480,Engine.Resources.GUI.Options));

//		this->Add_Button(Button_Var_Slider(-200,22,"Sensitivity",&APP.Options.Sensitivity));
	}
	else if(Menu=="update")
	{
//		std::stringstream Pass_Cmd;Pass_Cmd<<"LOL "<<SURVIVOR_VERSION<<" UPDATE";
//		CreateProcess("SurvivorUpdater.exe ",const_cast<char *>(Pass_Cmd.str().c_str()),NULL,NULL,TRUE,NORMAL_PRIORITY_CLASS,NULL,NULL,&APP.UpdaterStartInfo,&APP.UpdaterInfo);
//		APP.Quit("safe/Updating");
	}

	if(Go_Back)
	{
		this->Add_Button(Button_Menu(-200,96,"Back","back"));
	}
	
	this->Menu_Trace.push_back(Menu);
}								 */