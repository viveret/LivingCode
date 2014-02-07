#include "Object.h"
using namespace LC_Object;

#include "Engine.h"
extern S_Engine Engine;


#define NULL 0
#include "Draw_Utility.h"


Object_Base::Object_Base()// is an object, not ground -> ie grass, rock, ai
{							
	this->Delete_Flag = false;
	this->Deleted_Draw = false;
	this->Delete_Ready = false;

	this->Gravity = true;
	this->Static = true;	
					
	this->Size.x = 1;
	this->Size.y = 1;

	this->Type = TYPE_UNKNOWN;
	this->Tex = 0;
}

void Object_Base::Delete()
{

}

void Object_Base::Draw( bool Used_Color )
{
	if( Used_Color == false && this->Tex )// did not use custom color
		glColor4f( 1,1,1,1 );

	if( this->Tex )
	{			  
		Engine.Resources.Use_Texture( this->Tex );
	}
	else
	{
		sglDisable( GL_TEXTURE_2D );
	}


	glBegin( GL_QUADS );					   
		glTexCoord2f(0, 1);glVertex2f( this->Position.x, this->Position.y );
		glTexCoord2f(1, 1);glVertex2f( this->Position.x + this->Size.x, this->Position.y );
		glTexCoord2f(1, 0);glVertex2f( this->Position.x + this->Size.x, this->Position.y + this->Size.y );
		glTexCoord2f(0, 0);glVertex2f( this->Position.x, this->Position.y + this->Size.y );
	glEnd();			  
}

void Object_Base::Update( float dt )
{
	dt *= OBJ_SPEED;

	this->Velocity.y += dt * OBJ_GRAVITY;

	this->Position.x += this->Velocity.x * dt;
	this->Position.y += this->Velocity.y * dt;

	if( this->Position.y < 0 )
	{
		this->Position.y = 0;
		this->Velocity.y = -this->Velocity.y * OBJ_BOUNCE;
	}
	if( Position.x < 0.f )
	{
		this->Position.x = 0.f;
		this->Velocity.x = 0.f;
	}
	if( Position.x > Engine.Game.World_W )
	{
		this->Position.x = Engine.Game.World_W;
		this->Velocity.x = 0.f;
	}

	if( this->Velocity.x != 0.f )
	{
		signed char Old_Unit = RoundToUnit( this->Velocity.x );
		this->Velocity.x -= RoundToUnit( this->Velocity.x ) * OBJ_DRAG * dt; 

		if( RoundToUnit(this->Velocity.x) != Old_Unit )
			this->Velocity.x = 0.f;
	}
									   
//	this->Velocity.x *= OBJ_DRAG_RES * dt;
	//this->Velocity.y *= OBJ_DRAG_RES * dt;
}

void Object_Base::Apply_Force( Vector2 Force, float dt )
{									  
	this->Velocity.x += Force.x * dt * OBJ_SPEED;
	this->Velocity.y += Force.y * dt * OBJ_SPEED;
}