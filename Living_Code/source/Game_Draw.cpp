#include "Game.h"
using namespace E_Game;

#include "Engine.h"
extern S_Engine Engine;

#include "Draw_Utility.h"
using namespace Draw_Utility;
	 

#include "Colors.h"	  



void G_Game::Draw()
{
	glPushMatrix();

	glClear(GL_DEPTH_BUFFER_BIT);//| GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

	glScalef( 32, 32, 1 );	



							  						 
	glColor3f( 1,1,1 );	
	//sglDisable( GL_TEXTURE_2D );	
						
	//glColor3ub( GRASS_R, GRASS_G, GRASS_B );		   
	//Draw_Quad(0,6,60,1);	 
	Engine.Resources.Use_Texture( Engine.Resources.Game_Stuff.Grass );	 
	glBegin( GL_QUADS );
		glTexCoord2f( this->Player.Camera_Pos.x/4.f, 0 );glVertex2f( 0, 7 );
		glTexCoord2f( this->Player.Camera_Pos.x/4.f + 60.f/4, 0 );glVertex2f( 60, 7 );
		glTexCoord2f( this->Player.Camera_Pos.x/4.f + 60.f/4, 1.f );glVertex2f( 60, 6 );
		glTexCoord2f( this->Player.Camera_Pos.x/4.f, 1.f );glVertex2f( 0, 6 );
	glEnd();

	
	//glColor3ub( DIRT_R, DIRT_G, DIRT_B );						

	Engine.Resources.Use_Texture( Engine.Resources.Game_Stuff.Dirt );	 
	glBegin( GL_QUADS );
		glTexCoord2f( this->Player.Camera_Pos.x/16.f, 0 );glVertex2f( 0, 6 );
		glTexCoord2f( this->Player.Camera_Pos.x/16.f + 60.f/16, 0 );glVertex2f( 60, 6 );
		glTexCoord2f( this->Player.Camera_Pos.x/16.f + 60.f/16, 6.f/4 );glVertex2f( 60, 0 );
		glTexCoord2f( this->Player.Camera_Pos.x/16.f, 6.f/4 );glVertex2f( 0, 0 );
	glEnd();
			 

	sglEnable( GL_LIGHTING );																		 // + this->Player.Camera_Pos.x - ScreenW/64.f, + ScreenH/64.f
	Engine.Application.OpenGL.GL_Shader->Change(GL_LIGHTING,Vector3((Engine.Application.Input.Mouse_X/ScreenW) * ScreenW/32.f/32.f, 0.25f,0));


	this->Move_Perspective();

	glPushMatrix();				   
	//sglDisable( GL_TEXTURE_2D );	
	//draw barriers				
	//glColor3ub( DIRT_R, DIRT_G, DIRT_B );

	glRotatef( 45, 0, 0, 1 );		   
	glBegin( GL_QUADS );
		glTexCoord2f( 0, 0 );glVertex2f( -60, 50 );
		glTexCoord2f( 60.f/16, 0 );glVertex2f( 0, 50 );
		glTexCoord2f( 60.f/16, 60.f/4 );glVertex2f( 0, -10 );
		glTexCoord2f( 0, 60.f/4 );glVertex2f( -60, -10 );
	glEnd();
	
	glRotatef( -45, 0, 0, 1 );
	glTranslatef( this->World_W - 6.f, -10, 0 );
	glRotatef( -45, 0, 0, 1 );
	glBegin( GL_QUADS );
		glTexCoord2f( 0, 0 );glVertex2f( 0, 50 );
		glTexCoord2f( 60.f/16, 0 );glVertex2f( 60, 50 );
		glTexCoord2f( 60.f/16, 66.f/4 );glVertex2f( 60, -10 );
		glTexCoord2f( 0, 66.f/4 );glVertex2f( 0, -10 );
	glEnd();

	glPopMatrix();


			  


	for( u_int i = 0;i < this->AIS.size(); i++)
		this->AIS[i].Draw();


	if( this->Player.Highlighting )
	{
		sglDisable( GL_TEXTURE_2D );
		
		glColor4f( 1,0,0,0.3f );

		glBegin( GL_QUADS );																					 
			glVertex2f( this->Player.Highlighting->Phys.Position.x, this->Player.Highlighting->Phys.Position.y + this->Player.Highlighting->Phys.Size.y );
			glVertex2f( this->Player.Highlighting->Phys.Position.x + this->Player.Highlighting->Phys.Size.x, this->Player.Highlighting->Phys.Position.y + this->Player.Highlighting->Phys.Size.y );
			glVertex2f( this->Player.Highlighting->Phys.Position.x + this->Player.Highlighting->Phys.Size.x, this->Player.Highlighting->Phys.Position.y );
			glVertex2f( this->Player.Highlighting->Phys.Position.x, this->Player.Highlighting->Phys.Position.y );	
		glEnd();
	}

						
	this->Obj_Mem_Lock = true;
	while( this->Obj_Mem_Used );
	for( u_int i = 0;i < this->Objects.size(); i++)
	{
		this->Objects[i].Draw();		
	}	  
	this->Obj_Mem_Lock = false; 



	
	this->Particle_Mem_Locked = true;
	while( this->Particle_Mem_Used );

	this->Test_PSys.Draw();

	for(u_int i = 0;i < this->Particle_Systems.size(); i++)
		this->Particle_Systems[i].Draw();  

	this->Particle_Mem_Locked = false; 
									 


	//sglDisable( GL_LIGHTING );


	glPopMatrix();
}
				   


void G_Game::Move_Perspective()
{
	// now move to the ground
	glTranslatef( 0, 6.5, 0 );

	if( this->Player.Controling )
	{	  
		this->Player.Camera_Pos = this->Player.Controling->Phys.Position;  
		this->Player.Camera_Pos.x += this->Player.Controling->Phys.Size.x/2.f;
		this->Player.Camera_Pos.y += this->Player.Controling->Phys.Size.y/2.f;		


		this->Player.Controling->Looking_At = Vector2((Engine.Application.Input.Mouse_X/32.f - ScreenW/64.f),
			                                       (Engine.Application.Input.Mouse_Y + 280)/32.f - ScreenH/64.f);

		float Mag = this->Player.Controling->Looking_At.Magnitude();
		if( Mag > 1.f )
		{											   
			this->Player.Controling->Looking_At.x/=Mag;
			this->Player.Controling->Looking_At.y/=Mag;
		}


	}
	glTranslatef( -this->Player.Camera_Pos.x + ScreenW/64.f, 0, 0 );
}		


#define MIPMAP_SCALE 1.5f

#define MINIMAP_X ScreenW - Engine.Game.World_W*MIPMAP_SCALE - 12
#define MINIMAP_Y ScreenH - 20
#define MINIMAP_H 20


void G_Game::Draw_HUD()
{
	glPushMatrix();
	
	
	glColor4f( 1,1,1, 1.f-this->Player.Help_Show_Anim/1000.f );
	Engine.Resources.Use_Texture( Engine.Resources.Game_Stuff.Clouds );	 
	glBegin( GL_QUADS );
		glTexCoord2f( this->Player.Camera_Pos.x/16.f + this->Cloud_Movement, 3.f/4*(1.f-this->Player.Help_Show_Anim/600.f) );               glVertex2f( 0, ScreenH );
		glTexCoord2f( this->Player.Camera_Pos.x/16.f + ScreenW/512.f + this->Cloud_Movement, 3.f/4*(1.f-this->Player.Help_Show_Anim/600.f) );glVertex2f( ScreenW, ScreenH );
		glTexCoord2f( this->Player.Camera_Pos.x/16.f + ScreenW/512.f + this->Cloud_Movement, 1.f );  glVertex2f( ScreenW, ScreenH - 96 - this->Player.Help_Show_Anim );
		glTexCoord2f( this->Player.Camera_Pos.x/16.f + this->Cloud_Movement, 1.f );                 glVertex2f( 0, ScreenH - 96 - this->Player.Help_Show_Anim );
	glEnd();
	
	glColor4f( 1,1,1,1 );
	Engine.Resources.Use_Texture( Engine.Resources.Screen_Stuff.Help );

	glBegin( GL_QUADS );					   
		glTexCoord2f(0, 1);glVertex2f( ScreenW/2.f - 256, ScreenH - this->Player.Help_Show_Anim );
		glTexCoord2f(1, 1);glVertex2f( ScreenW/2.f + 256, ScreenH - this->Player.Help_Show_Anim );
		glTexCoord2f(1, 0);glVertex2f( ScreenW/2.f + 256, ScreenH - this->Player.Help_Show_Anim + 512 );
		glTexCoord2f(0, 0);glVertex2f( ScreenW/2.f - 256, ScreenH - this->Player.Help_Show_Anim + 512 );
	glEnd();



	sglDisable( GL_TEXTURE_2D );	


	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glColor3f( 0,0,0 );
	glBegin( GL_QUADS );				   
		glVertex2f( MINIMAP_X - 6, MINIMAP_Y - 6 );
		glVertex2f( MINIMAP_X - 6, MINIMAP_Y + MINIMAP_H );
		glVertex2f( MINIMAP_X + this->World_W*MIPMAP_SCALE + 6, MINIMAP_Y + MINIMAP_H );
		glVertex2f( MINIMAP_X + this->World_W*MIPMAP_SCALE + 6, MINIMAP_Y - 6 );
	glEnd();									 
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );



	for( u_int i = 0;i < this->AIS.size(); i++)
	{
		if( this->AIS[i].Health > 0 )
			glColor3f( 1, 0, 0 );
		else
			glColor3f( 0,0,1 );//0.2f, 0.2f, 0.4f );

		Draw_Quad( MINIMAP_X + this->AIS[i].Phys.Position.x*MIPMAP_SCALE - this->AIS[i].Phys.Size.x/2.f*MIPMAP_SCALE, MINIMAP_Y + this->AIS[i].Phys.Position.y*MIPMAP_SCALE - this->AIS[i].Phys.Size.y/2.f*MIPMAP_SCALE, this->AIS[i].Phys.Size.x*MIPMAP_SCALE, this->AIS[i].Phys.Size.y*MIPMAP_SCALE );
	}

	
	for( u_int i = 0;i < this->Objects.size(); i++)
	{
		switch( this->Objects[i].Type )
		{		  
		case LC_Object::TYPE_GRASS:
			glColor3ub( GRASS_R, GRASS_G, GRASS_B );	
			break;

		default:
			glColor3f( 0.5f, 0.5f, 0.5f );
			break;
		}

		Draw_Quad( MINIMAP_X + this->Objects[i].Position.x*MIPMAP_SCALE - this->Objects[i].Size.x/2.f*MIPMAP_SCALE, MINIMAP_Y + this->Objects[i].Position.y*MIPMAP_SCALE - this->Objects[i].Size.y/2.f*MIPMAP_SCALE, this->Objects[i].Size.x*MIPMAP_SCALE, this->Objects[i].Size.y*MIPMAP_SCALE );
	}

	glColor3f( 0, 0, 0 );
	Draw_Quad( MINIMAP_X + this->Player.Camera_Pos.x*MIPMAP_SCALE - 5, MINIMAP_Y + this->Player.Camera_Pos.y*MIPMAP_SCALE + 9, 6*MIPMAP_SCALE, 6*MIPMAP_SCALE );

		//SDraw<<"AX: "<<this->AIS.Player.Ai->Acceleration.x<<", AY: "<<this->AIS.Player.Ai->Acceleration.y<<", AZ: "<<this->AIS.Player.Ai->Acceleration.z;

		//SDraw<<"1: "<<this->Awd1<<", 2:"<<this->Awd2<<", 3:"<<this->Awd3<<", 4:"<<this->Awd4<<", 5:"<<this->Awd5;//<<", :"<<this->Awd1<<", :";	   
		//String_Draw(SDraw.str(),320,480,1.5,0,1,0,1);
		//SDraw.str("");


		//float Angle = (this->AIS.Player.Ai->Facing.z) * 3.141592654 / 180.;
		//SDraw<<"RotY: "<<this->AIS.Player.Ai->Facing.z<<", sin: "<<sin(Angle)<<", tan: "<<tan(Angle);

		//String_Draw(SDraw.str(),0,560,0,1,0,1);

		//glTranslatef(100,100,0);
		//Engine.Resources.Items.Draw_Icon( this->AIS.Player.Ai->Backpack.Hotbar.Get(0,0) );
		//glTranslatef(-100,-100,0);
	glPopMatrix();
}