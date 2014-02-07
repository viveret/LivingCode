#include "Resources.h"
using namespace E_Resources;

#include "Engine.h"
extern S_Engine Engine;

#include "Draw_Utility.h"
														
												
#define LOAD_TEX(X) Engine.Resources.Load_Texture(X)
#define LOAD_TEXF(X,Y) Engine.Resources.Load_Texture(X,Y)

#define LOAD_SND Engine.Resources.Load_Sound


#include <fstream>


//#pragma comment ( lib, "lib/libSOIL.lib" )


R_Screen_Resources::R_Screen_Resources()
{			
	this->Help = 0;
}

void R_Screen_Resources::Load_Stuff()
{	 
	this->Help = LOAD_TEX("stuff/screen/help.png");
}

R_Game_Resources::R_Game_Resources()
{			  	 	   
	this->AI_Body = 0;		  
	this->AI_Mouth = 0;
	this->AI_Poke = 0;

	this->AI_Eyes_Default = 0;	   
	this->AI_Eyes_Happy = 0;
	this->AI_Eyes_Sad = 0;
	this->AI_Eyes_Dead = 0;
			  
	this->Shrub = 0;
	this->Dirt = 0;
	this->Grass = 0;
	this->Clouds = 0;

	this->Moon = 0;
	this->Sun = 0;
	this->Clouds = 0;
	//skybox
	this->Down,Up,Side = 0;
	this->Sky_Tex = 0;
};

void R_Game_Resources::Load_Stuff()
{		 									 
	this->AI_Body = LOAD_TEX("stuff/body.png");	  
	this->AI_Mouth = LOAD_TEX("stuff/mouth.png"); 
	this->AI_Poke = LOAD_TEX("stuff/poke.png"); 

	this->AI_Eyes_Default = LOAD_TEX("stuff/eyes.png");			
	this->AI_Eyes_Happy = LOAD_TEX("stuff/eyes_happy.png");
	this->AI_Eyes_Sad = LOAD_TEX("stuff/eyes_sad.png");
	this->AI_Eyes_Dead = LOAD_TEX("stuff/eyes_dead.png");
										   
	this->Shrub = LOAD_TEX("stuff/shrub.png");								
	this->Dirt = LOAD_TEXF("stuff/dirt.png", SOIL_FLAG_TEXTURE_REPEATS);
	this->Grass = LOAD_TEXF("stuff/grass.png", SOIL_FLAG_TEXTURE_REPEATS);
	this->Clouds = LOAD_TEXF("stuff/clouds.png", SOIL_FLAG_TEXTURE_REPEATS);
}		

id R_Game_Resources::Get_Terrain(id ID)
{
	if( ID < this->Terrain.size() )
		return this->Terrain[ID];
	else 
		return 0;
}





R_Resources::R_Resources()
{
	this->Error = 0;
	this->Text = 0;


	this->Loaded_Necesities=false;
	this->Load_Fast = false;

	this->Current_Texture=0;
}

R_Resources::~R_Resources()
{
	//
}

void R_Resources::Load_Stuff()
{											   
	this->Error = LOAD_TEX("stuff/error.png");
	this->Text = LOAD_TEX("stuff/text.png");


	this->Screen_Stuff.Load_Stuff();
	this->Game_Stuff.Load_Stuff();
}
									  
id R_Resources::Load_Texture( std::string Filename, u_int oFlags )
{
	u_int Flags = SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | oFlags | SOIL_FLAG_POWER_OF_TWO;

	if(this->Load_Fast == true)
		Flags = Flags | SOIL_FLAG_COMPRESS_TO_DXT;


	GLuint nID=SOIL_load_OGL_texture
    (
      Filename.c_str(),
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
	  Flags
    );				 

	if( Engine.OpenGL_Assert( "Error with PNG texture: "+Filename,2 ) )
	{
		Engine.Application.Load_Log.Write("Failed\n");
		return 0;
	}

	//loaded successfully

	std::stringstream Report;Report<<"ID: "<<this->Images.size() << ", GL_ID: " << nID << "\n";
	Engine.Application.Load_Log.Write(Report.str());

	this->Current_Texture = this->Images.size();

	return this->Add_Tex(nID);//return index of image
}

id R_Resources::Load_Sound(std::string Filename)//draw splash screen too
{
	HSAMPLE nSamp=BASS_SampleLoad(FALSE,Filename.c_str(),0,0,MAX_PLAY_TIME,BASS_SAMPLE_3D);

	int nErr=BASS_ErrorGetCode();
	if(nErr!=0)
	{
//		std::stringstream nErrString;nErrString<<What<<" Had Error "<<nErr;
//		APP.ErrorShow.push_back(nErrString.str());
//		APP.Log.Write(nErrString.str().c_str());
		return 0;
	}

	//loaded successfully

	this->Samples.push_back(nSamp);
	
	std::stringstream Report;Report<<"Loaded Sound: "+Filename+", ID: "<<this->Samples.size()-1;
	Engine.Application.Load_Log.Write(Report.str());
	return this->Samples.size()-1;
}

void R_Resources::Use_Texture(id ID)
{						
	if(this->Current_Texture!=ID)//if it is different
	{
		if(ID<this->Images.size())//within range
		{				 
			Engine.Application.OpenGL.GL_Shader->Enable(GL_TEXTURE_2D);

			this->Current_Texture=ID;
			glBindTexture(GL_TEXTURE_2D,this->Images[ID]);
		}
		else
			//failed to load
			this->Current_Texture=0;
	}
	else 
		Engine.Application.OpenGL.GL_Shader->Enable(GL_TEXTURE_2D);
}

GLuint R_Resources::Get_Tex(id ID)
{
	if( ID < this->Images.size() )
		return this->Images[ID];
	else
		return 0;
}

id R_Resources::New_Tex()
{					
	GLuint n_Tex = 0;
	glGenTextures( 1, &n_Tex );

	return this->Add_Tex(n_Tex);
}

id R_Resources::Add_Tex(GLuint ID)
{				  
	this->Images.push_back( ID );
	return this->Images.size()-1;
}
		  /*
void R_Main_Container::Start_Render(u_int Tex_X, u_int Tex_Y, u_int Tex_W, u_int Tex_H, id Tex_ID)
{
	if( this->Render_Tex_ID != 0)
		return;// return if already rendering

	this->Render_Tex_ID = Tex_ID;	  
	
	this->Render_Tex_X = Tex_X;
	this->Render_Tex_Y = Tex_Y;
	this->Render_Tex_W = Tex_W;
	this->Render_Tex_H = Tex_H;
}

void R_Main_Container::End_Render(char a)
{
	if( this->Render_Tex_ID == 0)
		return;// if was not rendering


	sglEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, Engine.Resources.Stuff.Get_Tex( this->Render_Tex_ID ) );

	GLuint *Pixels = new GLuint[ this->Render_Tex_W * this->Render_Tex_H * 4 ];
	memset(Pixels, 255, this->Render_Tex_W * this->Render_Tex_H * 4 );


	glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, this->Render_Tex_W, this->Render_Tex_H, GL_RGBA, GL_UNSIGNED_BYTE, Pixels );		   
	Engine.Opengl_Error(S_Error("Failed to texsubimage",4));
	//glCopyTexSubImage2D( GL_TEXTURE_2D, 0, this->Render_Tex_X, this->Render_Tex_Y, 0, Engine.Application.Window.Wind_h - this->Render_Tex_H, this->Render_Tex_W, this->Render_Tex_H);		   
	//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 512, 512, 0);	  
	gluBuild2DMipmaps( GL_TEXTURE_2D, 4, this->Render_Tex_W, this->Render_Tex_H, GL_RGBA, GL_UNSIGNED_BYTE, Pixels);
	Engine.Opengl_Error(S_Error("Failed to build mipmaps",4));

	delete[] Pixels;

	glBindTexture( GL_TEXTURE_2D, 0 );

	this->Render_Tex_X = 0;
	this->Render_Tex_Y = 0;
	this->Render_Tex_W = 0;
	this->Render_Tex_H = 0;
	this->Render_Tex_ID = 0;
}		*/


		  /*
void R_AI_Container::Init(std::string Folder)
{
	this->Name=Folder;
	this->Tex_ID=LOAD_TEX_BMP("stuff/ai/"+Folder+"/texture.bmp");
}

std::string R_Screen_GUI::Initialize()
{																									
	this->Background=LOAD_TEX_BMP("stuff/screen/gui/screen/screen_background.bmp");	  /*
	this->End=LOAD_TEX_PNG("stuff/screen/gui/screen/end.png"); 	 
	this->Inventory=LOAD_TEX_PNG("stuff/screen/gui/screen/Inventory.png");	 
	this->Saving=LOAD_TEX_PNG("stuff/screen/gui/screen/save.png");
	this->Loading=LOAD_TEX_PNG("stuff/screen/gui/screen/load.png");
	this->Online=LOAD_TEX_PNG("stuff/screen/gui/screen/online.png"); 
	this->Options=LOAD_TEX_PNG("stuff/screen/gui/screen/options.png");
	this->Pause=LOAD_TEX_PNG("stuff/screen/gui/screen/pause.png");
	this->Play_Game=LOAD_TEX_PNG("stuff/screen/gui/screen/play_game.png");
	this->Title=LOAD_TEX_PNG("stuff/screen/gui/screen/title.png");	 */		  /*
	//
	this->Click=LOAD_SND("stuff/screen/gui/sounds/mouse_click.wav");
	//	  
	this->Backpack_Holder=LOAD_TEX_PNG("stuff/screen/gui/backpack_holder.png");		
	this->Buttons=LOAD_TEX_PNG("stuff/screen/gui/buttons.png");
	this->Scroll_Buttons=LOAD_TEX_PNG("stuff/screen/gui/scroll_butts.png");
	this->Char_Custimize=LOAD_TEX_PNG("stuff/screen/gui/char_custimize.png");

	return "OK";
}

std::string R_Screen_HUD::Initialize()
{					
	this->Visor=LOAD_TEX_PNG("stuff/screen/hud/screen/visor.png");
	this->Chat_Bar=LOAD_TEX_PNG("stuff/screen/hud/screen/chat_bar.png");
	this->Chat_Box=LOAD_TEX_PNG("stuff/screen/hud/screen/chat_box.png");
	this->Convo_Box=LOAD_TEX_PNG("stuff/screen/hud/screen/convo_box.png");
	this->Dark_View=LOAD_TEX_PNG("stuff/screen/hud/screen/dark_view.png");	
	this->Death=LOAD_TEX_PNG("stuff/screen/hud/screen/death.png");
	this->Pick_Up=LOAD_TEX_PNG("stuff/screen/hud/screen/pick_up.png");
	this->R_Side=LOAD_TEX_PNG("stuff/screen/hud/screen/r_side.png");
	this->Top=LOAD_TEX_PNG("stuff/screen/hud/screen/top.png");	 
	this->Tools=LOAD_TEX_PNG("stuff/screen/hud/screen/tools.png");
	this->Tools = LOAD_TEX_PNG("stuff/screen/hud/screen/hud.png");
	//
	this->Change_Item=LOAD_SND("stuff/screen/hud/sounds/change_item.wav"); 
	this->Chat_Toggle=LOAD_SND("stuff/screen/hud/sounds/chat_toggle.png");
	//							 													 
	this->Attributes=LOAD_TEX_PNG("stuff/screen/hud/attributes.png");
	this->Checkpoint=LOAD_TEX_PNG("stuff/screen/hud/checkpoint.png"); 
	this->Crosshair=LOAD_TEX_PNG("stuff/screen/hud/crosshair.png");	 
	this->Bar_Energy=LOAD_TEX_PNG("stuff/screen/hud/energy_bar.png");
	this->Bar_Health=LOAD_TEX_PNG("stuff/screen/hud/health_bar.png"); 
	this->Watch=LOAD_TEX_PNG("stuff/screen/hud/hud_watch.png");	 
	this->Pointer=LOAD_TEX_PNG("stuff/screen/hud/pointer.png");

	return "OK";
}

std::string R_Env_Obj::Initialize()
{
	//	  
	return "OK";
}

id R_Environment_Stuff::Get_Terrain(id ID)
{
	if(ID<this->Terrain.size())
		return this->Terrain[ID];
	else 
		return 0;
}

std::string R_Environment_Stuff::Initialize()
{
	unsigned int TexID=0;
	std::stringstream nFile;nFile<<"stuff/environment/normal/"<<TexID<<".png";
	while(std::ifstream(nFile.str()))
	{
		this->Terrain.push_back(LOAD_TEX_PNG(nFile.str()));
		TexID++;
		nFile.str("");nFile<<"stuff/environment/normal/"<<TexID<<".png";
	}
	//																		
	this->Sun=LOAD_TEX_PNG("stuff/environment/sky/sun.png");
	this->Moon=LOAD_TEX_PNG("stuff/environment/sky/moon.png"); 
	this->Clouds=LOAD_TEX_PNG("stuff/environment/sky/clouds.png");
	this->Down=LOAD_TEX_PNG("stuff/environment/sky/sky_b.png");
	this->Up=LOAD_TEX_PNG("stuff/environment/sky/sky_t.png");	  
	this->Side=LOAD_TEX_PNG("stuff/environment/sky/sky_s.png");

	this->Sky_Tex=LOAD_TEX_BMP("stuff/environment/sky/sky_tex.bmp");

	return "OK";
}

std::string R_Item_Folder::Initialize(std::string Folder)
{																					 
	this->Texture=LOAD_TEX_PNG("stuff/items/"+Folder+"/texture.png");
	//																				 
	this->Consume=LOAD_SND("stuff/items/"+Folder+"/consume.wav");
	this->Reload=LOAD_SND("stuff/items/"+Folder+"/reload.wav");

	return "OK";
}

#include "Utility.h"

R_Item_Stuff::R_Item_Stuff()
{
	//sounds
	this->Swoosh = 0;//default swing
	this->Empty;//gun was empty
	this->Shell_Drop;//dropped shell
	//images
	this->Icon_Ranged = 0;
	this->Icon_Mellee = 0;
	this->Icon_Vanity = 0;
	this->Icon_Ammo = 0;
	this->Icon_Attribute = 0;//icons for items

	//skins
	this->Grey_Skin = 0;

	//icons for items
	this->Item_Icons = 0;
	memset( this->Icon_Cache, 0, ICON_SIZE * ICON_SIZE * sizeof(bool) );//to tell which icons are in use
}

std::string R_Item_Stuff::Initialize()
{										
	unsigned int ItemID=0;
	std::stringstream nFolder;nFolder<<"ranged_"<<ItemID;
	while(dirExists("stuff/items/"+nFolder.str()))
	{
		R_Item_Folder nIFold;nIFold.Initialize(nFolder.str());
		this->Ranged.push_back(nIFold);
		ItemID++;
		nFolder.str("");
		nFolder<<"ranged_"<<ItemID;
	}
	//
	ItemID=0;nFolder.str("");
	nFolder<<"mellee_"<<ItemID;
	while(dirExists("stuff/items/"+nFolder.str()))
	{
		R_Item_Folder nIFold;nIFold.Initialize(nFolder.str());
		this->Mellee.push_back(nIFold);
		ItemID++;
		nFolder.str("");
		nFolder<<"mellee_"<<ItemID;
	}
	//
	ItemID=0;nFolder.str("");
	nFolder<<"vanity_"<<ItemID;
	while(dirExists("stuff/items/"+nFolder.str()))
	{
		R_Item_Folder nIFold;nIFold.Initialize(nFolder.str());
		this->Vanity.push_back(nIFold);
		ItemID++;
		nFolder.str("");
		nFolder<<"vanity_"<<ItemID;
	}
	//	  
	ItemID=0;nFolder.str("");
	nFolder<<"ammo_"<<ItemID;
	while(dirExists("stuff/items/"+nFolder.str()))
	{
		R_Item_Folder nIFold;nIFold.Initialize(nFolder.str());
		this->Ammo.push_back(nIFold);
		ItemID++;
		nFolder.str("");
		nFolder<<"ammo_"<<ItemID;
	}
	//					
	ItemID=0;nFolder.str("");
	nFolder<<"attribute_"<<ItemID;
	while(dirExists("stuff/items/"+nFolder.str()))
	{
		R_Item_Folder nIFold;nIFold.Initialize(nFolder.str());
		this->Attribute.push_back(nIFold);
		ItemID++;
		nFolder.str("");
		nFolder<<"attribute_"<<ItemID;
	}
	//											  
	this->Clean_Ranged.Initialize("clean_ranged");
	this->Clean_Mellee.Initialize("clean_mellee");
	//sounds																   
	this->Swoosh=LOAD_SND("stuff/items/common/swoosh.wav"); 
	this->Empty=LOAD_SND("stuff/items/common/empty.wav");
	this->Shell_Drop=LOAD_SND("stuff/items/common/shell_drop.wav");
	/*//*icons															
	this->Icon_Ranged=LOAD_TEX_PNG("stuff/items/icons/ranged.png");
	this->Icon_Mellee=LOAD_TEX_PNG("stuff/items/icons/mellee.png");
	this->Icon_Vanity=LOAD_TEX_PNG("stuff/items/icons/vanity.png");
	this->Icon_Ammo=LOAD_TEX_PNG("stuff/items/icons/ammo.png");
	this->Icon_Attribute=LOAD_TEX_PNG("stuff/items/icons/attribute.png");		  */	/*

	this->Grey_Skin = LOAD_TEX_BMP("stuff/items/skins/leather.bmp");



	this->Item_Icons = Engine.Resources.Stuff.New_Tex();

	glBindTexture( GL_TEXTURE_2D, Engine.Resources.Stuff.Get_Tex( this->Item_Icons ) );

	GLuint *Pixels = new GLuint[ ICON_SIZE * 32 * ICON_SIZE * 32 * 4];
	memset(Pixels, 100, ICON_SIZE * 32 * ICON_SIZE * 32 * 4);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	
	gluBuild2DMipmaps(GL_TEXTURE_2D,4, ICON_SIZE * 32, ICON_SIZE * 32,GL_RGBA,GL_UNSIGNED_BYTE,Pixels);



	return "OK";
}

void R_Item_Stuff::Add_Icon(E_Game::G_Items::I_Item_Base* What)
{			 
	Point2D Pos = this->GetXY( NULL );

	if( Pos.x == -1 )// no NULL found
		return;

	this->Icon_Cache[Pos.x][Pos.y] = What;
	this->Refresh_Icon( What );
}

void R_Item_Stuff::Remove_Icon(E_Game::G_Items::I_Item_Base* What)
{
	Point2D Pos = this->GetXY( What );

	if( Pos.x == -1 )// item not found
		return;

	this->Icon_Cache[Pos.x][Pos.y] = NULL;
}

void R_Item_Stuff::Remove_All_Icons()//remove all icons
{
	memset(this->Icon_Cache, 0, sizeof(E_Game::G_Items::I_Item_Base*));
}

void R_Item_Stuff::Refresh_Icon(E_Game::G_Items::I_Item_Base* What)
{
	u_int Icon_ID_Temp = this->Item_Icons;//store as temp to disable usage
	this->Item_Icons = 0;



	SuspendThread(Engine.Thread_Draw.hThread);


	Engine.Application.OpenGL.GL_RTT->Start_Render(32,32,Icon_ID_Temp);					  
	glTranslatef(0,1,-3);		 
	glRotatef(180,0,0,1);//glRotatef(180,0,1,0);


	Engine.Resources.Stuff.Use_Texture(Engine.Resources.AI[0].Tex_ID);
//	Engine.Content.AI_Meshes.Humanoid.Draw();		  

																	   
	glRotatef(-180,0,0,1);//glRotatef(-180,0,1,0);
	glTranslatef(0,-1,3);		 

	Icon_ID_Temp = Engine.Application.OpenGL.GL_RTT->End_Render();	   

	this->Item_Icons = Icon_ID_Temp;

	ResumeThread(Engine.Thread_Draw.hThread);
}

void R_Item_Stuff::Refresh_All_Icons()//refresh all to match current cache
{										
	for(u_char x = 0;x < ICON_SIZE; x++)
		for(u_char y = 0;y < ICON_SIZE; y++)
			this->Refresh_Icon( this->Icon_Cache[x][y] );
}

void R_Item_Stuff::Draw_Icon( E_Game::G_Items::I_Item_Base* What )
{
	Point2D Where = this->GetXY( What );

	if( Where.x == -1 )
		return;

	Engine.Resources.Stuff.Use_Texture( Item_Icons );

	glBegin(GL_QUADS);//back
		glTexCoord2f((float)Where.x / ICON_SIZE, Where.y / ICON_SIZE + 1.f/ICON_SIZE);glVertex2f(-16,16);
        glTexCoord2f(Where.x / ICON_SIZE + 1.f/ICON_SIZE, (float)Where.y / ICON_SIZE + 1.f/ICON_SIZE);glVertex2f(16,16);
        glTexCoord2f(Where.x / ICON_SIZE + 1.f/ICON_SIZE, (float)Where.y / ICON_SIZE);glVertex2f(16,-16);
        glTexCoord2f((float)Where.x / ICON_SIZE, (float)Where.y / ICON_SIZE);glVertex2f(-16,-16);
    glEnd();
}

Point2D R_Item_Stuff::GetXY( E_Game::G_Items::I_Item_Base* What )
{														  
	for( u_char x = 0;x < ICON_SIZE; x++ )
		for( u_char y = 0;y < ICON_SIZE; y++ )
			if( this->Icon_Cache[ x ][ y ] )
				return Point2D( x, y );

	return Point2D();
}



std::string R_Resources::Initialize()
{						 	
	Engine.Application.Main_Log.Write_Log("Loading resources...");
	Engine.Application.Load_Log.Write_Log("Loading prerequisit resources...");

	this->Error=this->Stuff.Load_Texture_PNG("stuff/screen/error.png");
	this->Text=this->Stuff.Load_Texture_PNG("stuff/screen/text.png");
	this->GUI.Splash=this->Stuff.Load_Texture_BMP("stuff/screen/gui/screen/splash.bmp");
	//done with necesities
	this->Stuff.Loaded_Necesities=true;

	std::string Report;//keep track of status

	Report=this->GUI.Initialize();
	if(Report!="OK"){return Report;}

	Report=this->HUD.Initialize();	 
	if(Report!="OK"){return Report;}
	//
	//ai
	//
	Report=this->Environment.Initialize();
	if(Report!="OK"){return Report;}

	Report=this->Items.Initialize();
	if(Report!="OK"){return Report;}

	/*   AIS   */			  /*

	R_AI_Container NewAI;
	NewAI.Init("humanoid/human_male");
	this->AI.push_back(NewAI);			 
	NewAI.Init("humanoid/zombie");
	this->AI.push_back(NewAI);
	NewAI.Init("humanoid/santa");
	this->AI.push_back(NewAI);

	this->Loading.clear();

																				 
	Engine.Application.Load_Log.Write_Log("Finished loading prerequisit resources.");
	Engine.Application.Main_Log.Write_Log("Successfully loaded resources.");

	return "OK";//everything went perfectly
}

void R_Resources::Pre_Render()
{			  
	//done initializing
	Engine.Application.Main_Log.Write_Log("Starting Pre-Render...");

	Engine.Application.OpenGL.GL_RTT->Start_Render(512,512);				  
	glTranslatef(0,1,-3);		 
	glRotatef(180,0,0,1);//glRotatef(180,0,1,0);


	this->Stuff.Use_Texture(this->AI[0].Tex_ID);
	//Engine.Content.AI_Meshes.Humanoid.Draw();		  

																	   
	glRotatef(-180,0,0,1);//glRotatef(-180,0,1,0);
	glTranslatef(0,-1,3);		 

	this->Test_Render = Engine.Application.OpenGL.GL_RTT->End_Render();	   
	Engine.Application.Main_Log.Write_Log("Pre-Render Complete");
}

/*    IMAGE GET FUNCS    */		 /*
id R_Resources::Show_To_Type(std::string Type)
{
	if(Type=="ranged"){return this->Items.Icon_Ranged;}
	else if(Type=="mellee"){return this->Items.Icon_Mellee;}
	else if(Type=="vanity"){return this->Items.Icon_Vanity;}
	else if(Type=="ammo"){return this->Items.Icon_Ammo;}
	else if(Type=="attribute"){return this->Items.Icon_Attribute;}
    else{return 0;}
}

POINT R_Resources::Type_To_At(std::string Type)
{
	unsigned int ID;
	if(Type=="knock_back"){ID=1;}
    if(Type=="vaporize"){ID=2;}

	POINT Ret;
	Ret.y=ID/16;Ret.x=ID-Ret.y*16;

	return Ret;
}

id R_Resources::Terrain(unsigned int ID)
{
	if(ID<this->Environment.Terrain.size())
	{
		return this->Environment.Terrain[ID];
	}
	else
	{
		return this->Error;
	}
}

R_AI_Container* R_Resources::Get_AI(std::string Name)
{
	for(unsigned int i=0;i<this->AI.size();i++)
	{
		if(this->AI[i].Name==Name)
		{
			return &this->AI[i];
		}
	}
	return &this->AI[0];//NULL;
}

R_Item_Folder R_Resources::Get_Item(std::string Type,id ID)
{
	if(true)//!APP.Options.Clean)
    {
		if(Type=="ranged"){return this->Items.Ranged[ID];}
        else if(Type=="mellee"){return this->Items.Mellee[ID];}
        else if(Type=="vanity"){return this->Items.Vanity[ID];}
        else if(Type=="ammo"){return this->Items.Ammo[ID];}
        else if(Type=="attribute"){return this->Items.Attribute[ID];}
        else
        {
			R_Item_Folder Default;Default.Consume=0;Default.Reload=0;Default.Texture=0;
			return Default;
            //return 0;
        }
    }
    else if(false)//APP.Options.Clean)
    {
        /*if(type=="ranged")
        {
            return ImageHolder::Items.Clean_Ranged;
        }
        else if(type=="mellee")
        {
            return ImageHolder::Items.Clean_Mellee;
        }
        else if(type=="attribute")
        {
            return ImageHolder::Items.Attribute[1];
        }
        else
        {
            return 0;
        }	*/ 			/*
    }
}				 		  */