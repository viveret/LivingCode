#include "Game.h"
using namespace E_Game;

#include "GL\glew.h"
#include "GL\GL.h"	

#include "Engine.h"
extern S_Engine Engine;


G_Player::G_Player()
{
	this->Controling = NULL;
	this->Highlighting = NULL;

	this->Help_Show_Anim = 0.f;
	this->Help_Open = false;
}

G_Player::~G_Player()
{

}		

/*void G_World::Initialize()
{
	for(int my=0;my<3;my++)
    {
        for(int mx=0;mx<3;mx++)
        {
//            this->Environment[mx][my].Init();
        }
    }

	this->Cloud_Move=0;
}

void G_World::Destroy()
{
	//
}

void G_World::Make_Random()
{
	for(int my=0;my<3;my++)
    {
        for(int mx=0;mx<3;mx++)
        {
//			this->Environment[mx][my].newRandom();
        }
    }
}		*/


G_Game::G_Game()
{	 
	this->Test_PSys.Position.x = 10;
	this->Test_PSys.Position.y = 5;
	this->Test_PSys.Running = true;
							 	  
	this->Obj_Mem_Lock = false;
	this->Obj_Mem_Used = false;
	
	this->Particle_Mem_Locked = false;
	this->Particle_Mem_Used = false;


	this->Cloud_Movement = -MAX_CLOUD_MOVE;


	this->Grass_Duration = 0.f;

	this->World_W = 256;

	this->Grass_Max_Amount = 100;
	this->Grass_Spawn_Time = 200.f;

	this->Max_AIs = 7;

	this->Harshness = 0.6f;
}

G_Game::~G_Game()
{
	this->Destroy();
}		  






void G_Game::Initialize()
{			
	this->Player.Camera_Pos.x = this->World_W / 2.f;

	for(u_int i = 0;i < this->Max_AIs; i++)
	{
		AI::AI_Living nGuy;
		nGuy.Phys.Tex = Engine.Resources.Game_Stuff.AI_Body;
		nGuy.Phys.Position.x = rand()%this->World_W;

		this->AIS.push_back(nGuy); 
	}	  
}

void G_Game::Destroy()
{			   
	for(unsigned int a=0;a<this->AIS.size();a++)
		this->AIS[a].Phys.Delete();

	this->AIS.clear();
}

void G_Game::Spawn_Grass( float dt )
{
	this->Grass_Duration += dt * 0.02f;

	if( this->Grass_Duration > this->Grass_Spawn_Time && Grass_Amount < this->Grass_Max_Amount )
	{
		this->Grass_Duration = 0.f;
		this->Grass_Amount++;

		LC_Object::Object_Base nGrass;
		nGrass.Tex = Engine.Resources.Game_Stuff.Shrub;


		nGrass.Position.x = rand()%this->World_W;
		nGrass.Position.y = 0;
					 
		nGrass.Size.x = 4;
		nGrass.Size.y = 4;

		nGrass.Type = LC_Object::TYPE_GRASS;

		this->Objects.push_back( nGrass );
	}
}

#include "Colors.h"
void G_Game::Delete_Object( LC_Object::Object_Base* Which )
{
	u_int Index = this->Get_Obj_Index( Which );

	if( Index == unsigned(-1) )
		return;


	switch( this->Objects[Index].Type )
	{
	case LC_Object::TYPE_GRASS:
		{
			this->Grass_Amount--;

			E_Particle_System::P_Particle_System nGrass_Particles;
			nGrass_Particles.Position = this->Objects[Index].Position;
			nGrass_Particles.Position.y += 2.f;
			nGrass_Particles.Position.x += this->Objects[Index].Size.x/2.f;

			nGrass_Particles.Velocity = Vector2( 0, 0 );
			nGrass_Particles.Rand_Noise = 1.f;
			nGrass_Particles.Particles_Per_Sec = 10;
			nGrass_Particles.Particle_Duration = 7.f;

			nGrass_Particles.Start_Size = 0.5f;
			nGrass_Particles.Start_R = GRASS_R;
			nGrass_Particles.Start_G = GRASS_G;
			nGrass_Particles.Start_B = GRASS_B;
				
			nGrass_Particles.End_R = GRASS_R * 0.5f;
			nGrass_Particles.End_G = GRASS_G * 0.5f;
			nGrass_Particles.End_B = GRASS_B * 0.5f;

			nGrass_Particles.Running = true;

			this->Particle_Systems.push_back( nGrass_Particles );
			this->Particle_Life_Times.push_back( 400.f );
		}break;
	}

#ifdef PAUSE_DRAW_WHEN_UPDATE
	//SuspendThread( Engine.Thread_Draw.hThread );
#endif

	this->Objects[Index].Delete();
	this->Objects.erase( this->Objects.begin() + Index );

#ifdef PAUSE_DRAW_WHEN_UPDATE
	//ResumeThread( Engine.Thread_Draw.hThread );
#endif
}

u_int G_Game::Get_Obj_Index( LC_Object::Object_Base* Obj )
{
	for( u_int i = 0;i < this->Objects.size(); i++)
		if( Obj == &this->Objects[i] )
			return i;

	return -1;
}

LC_Object::Object_Base* G_Game::Get_Obj( u_int Type, u_int* Start_From )
{
	for(*Start_From+=1;*Start_From < this->Objects.size(); *Start_From+=1)
	{
		if( this->Objects[*Start_From].Type == Type )
		{
			//*Start_From-=1;
			return &this->Objects[*Start_From];
		}
	}


	return NULL;
}

u_int G_Game::Get_Obj_Index( u_int Type, u_int Start_From )
{	  
	for( u_int i = Start_From;i < this->Objects.size(); i++)
	{
		if( this->Objects[i].Type == Type )
			return i;
	}

	return -1;
}