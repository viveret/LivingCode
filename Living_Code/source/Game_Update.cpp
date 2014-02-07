#include "Game.h"
using namespace E_Game;

#include "Engine.h"
extern S_Engine Engine;

#include <limits>		  

#include "Draw_Utility.h"
#include "Colors.h"


void G_Player::Update(float dt)
{			 
	//if( Engine.Application.Input.XBOX.InUse )
	{																		 
		if( this->Controling )
		{
			float Move_X = Engine.Application.Input.XBOX.LAnol.x;
			if( Engine.Application.Input.Keys.Left.Down )
				Move_X -= 1.f;
			else if( Engine.Application.Input.Keys.Right.Down )
				Move_X += 1.f;

			if( abs(Move_X) > 1.f )
				Move_X = RoundToUnit( Move_X );

			this->Controling->Move( Move_X, dt );

			this->Controling->Consume_Energy( Engine.Application.Input.XBOX.LTrig * 50 );
			
			if( (Engine.Application.Input.XBOX.X || Engine.Application.Input.Keys.Eat.Down) && this->Controling )// clicked
			{
				this->Controling->Eat();
				Engine.Application.Input.XBOX.X = false;
			}

			if( Engine.Application.Input.XBOX.RAnolDown || Engine.Application.Input.Keys.Toggle_Think.Down )
			{
				this->Controling->Think = !this->Controling->Think;
				Engine.Application.Input.XBOX.RAnolDown = false;
			}

			
			if( this->Controling->Health <= 0 )
			{
				this->Controling->Draw_Info = false;
				this->Controling->Think = true;
				this->Controling = NULL;
			}
		}
		else
		{													  
			float Move_X = Engine.Application.Input.XBOX.LAnol.x;
			if( Engine.Application.Input.Keys.Left.Down )
				Move_X -= 1.f;
			else if( Engine.Application.Input.Keys.Right.Down )
				Move_X += 1.f;

			if( abs(Move_X) > 1.f )
				Move_X = RoundToUnit( Move_X );


			float Add_Cam = Move_X * dt * 0.03f;
								  
			if( Engine.Application.Input.XBOX.Pad_R )
				Add_Cam += 0.2f * dt;
			
			if( Engine.Application.Input.XBOX.Pad_L )
				Add_Cam -= 0.2f * dt;

			this->Camera_Pos.x += Add_Cam;
		}


		if( Engine.Application.Input.XBOX.Start )
		{
			this->Help_Open = !this->Help_Open;
			Engine.Application.Input.XBOX.Start = false;
		}
													  
		if( this->Help_Open )
		{
			if( this->Help_Show_Anim < 600.f )
				this->Help_Show_Anim += 0.8f * dt;
			
			if( this->Help_Show_Anim > 600.f )
				this->Help_Show_Anim = 600.f;
		}
		else
		{
			if( this->Help_Show_Anim > 0.f )
				this->Help_Show_Anim -= 0.8f * dt;
			
			if( this->Help_Show_Anim < 0.f )
				this->Help_Show_Anim = 0.f;
		}
	}
							 
	if( this->Camera_Pos.x < 0 )
		this->Camera_Pos.x = 0;

	if( this->Camera_Pos.x > Engine.Game.World_W )
		this->Camera_Pos.x = Engine.Game.World_W;


	this->Highlighting = NULL;
}

void G_Game::Update(float dt)
{
	this->Update_Particle_Systems( dt );

	this->Player.Update( dt );

	for(u_int i = 0;i < this->AIS.size(); i++)
	{
		this->AIS[i].Update( dt );
		if( Is_In_Box( this->AIS[i].Phys.Position.x, this->AIS[i].Phys.Position.y, this->AIS[i].Phys.Size.x, this->AIS[i].Phys.Size.y, 
			Engine.Application.Input.Mouse_X/32.f + this->Player.Camera_Pos.x - ScreenW/64.f, Engine.Application.Input.Mouse_Y/32.f ) && this->Player.Controling != &this->AIS[i] && this->AIS[i].Health > 0 )
		{
			this->Player.Highlighting = &this->AIS[i];
		}
	}

	if( Engine.Application.Input.Mouse_L_Down || Engine.Application.Input.XBOX.Y )// clicked
	{		  
		if( this->Player.Controling )
		{
			this->Player.Controling->Draw_Info = false;
			this->Player.Controling->Think = true;
		}

		if( this->Player.Highlighting )
		{	   
			this->Player.Controling = this->Player.Highlighting;
			this->Player.Controling->Think = false;
			this->Player.Controling->Draw_Info = true;
		}
		else
		{
			this->Player.Controling = NULL;
		}

		Engine.Application.Input.Mouse_L_Down = false;
		Engine.Application.Input.XBOX.Y = false;
	}

	if( Engine.Application.Input.XBOX.A && this->Player.Highlighting )
	{
		this->Player.Highlighting->Draw_Info = !this->Player.Highlighting->Draw_Info;
		Engine.Application.Input.XBOX.A = false;
	}


	this->Update_Objects( dt );
	this->Spawn_Grass( dt );

	this->Cloud_Movement += dt / MAX_CLOUD_MOVE * 0.1f;;
	if( this->Cloud_Movement > MAX_CLOUD_MOVE )
		this->Cloud_Movement = -MAX_CLOUD_MOVE;
}

void G_Game::Update_Objects( float dt )
{				  
	this->Obj_Mem_Used = true;	  
	while( this->Obj_Mem_Lock );// wait until not locked

	for( u_int i = 0;i < this->Objects.size(); i++)
	{
		this->Objects[i].Update( dt );
		
		if( this->Objects[i].Delete_Ready )
		{
			this->Delete_Object( &this->Objects[i] );
			i--;
		}	
		else if( this->Objects[i].Delete_Flag )// && this->Objects[i].Deleted_Draw )
		{
			this->Objects[i].Delete_Ready = true;
		}
	}	
	this->Obj_Mem_Used = false;	
}

void G_Game::Update_Particle_Systems( float dt )
{			
	this->Particle_Mem_Used = true;	  
	while( this->Particle_Mem_Locked );// wait until not locked

	this->Test_PSys.Update( dt );

	for(u_int i = 0;i < this->Particle_Systems.size(); i++)
	{
		this->Particle_Systems[i].Update( dt );
		this->Particle_Life_Times[i] -= dt;

		if( this->Particle_Life_Times[i] < 0 )
		{
			if( this->Particle_Systems[i].Particles.size() == 0 )
			{																		
				this->Particle_Systems.erase( this->Particle_Systems.begin() + i );
				this->Particle_Life_Times.erase( this->Particle_Life_Times.begin() + i );

				i--;
			}
			else
				this->Particle_Systems[i].Running = false;
		}
	}			
	this->Particle_Mem_Used = false;	
}