#pragma warning(disable:4996)

#include "AI.h"
#define SIZEOFMAP 100
#include <math.h>
#include <Windows.h>
#include "myMath.h"
#include <cstdlib>
#include <string.h>
#include <math.h>

//using namespace ModelHolder;

#include "Engine.h"
extern S_Engine Engine;

using namespace AI;

char Name_Vowel_Array[] = {'a','e','i','o','u'};																
char Name_Consonant_Array[] = {'b','c','d','g','j','k','p','q','t','v','w','x','y','z'};
char Name_Bridge_Array[] = {'f','h','l','m','n','r','s','-','\''};


char Name_Vowel()
{
	return Name_Vowel_Array[ rand()%sizeof(Name_Vowel_Array) ];
}

char Name_Consonant()
{
	return Name_Consonant_Array[ rand()%sizeof(Name_Consonant_Array) ];
}

char Name_Bridge()
{
	return Name_Bridge_Array[ rand()%sizeof(Name_Bridge_Array) ];
}

std::string Name_Part()
{
	std::string Ret;

	u_char Length = rand()%5 + 5;
									 
	u_char Letter_State = rand()%3;
	for(u_char i = 0;i < Length; i++)
	{		
		switch( Letter_State )
		{		
		case 0:Ret += Name_Consonant(); break;
		case 1:
			{
				Ret += Name_Vowel();
				Ret += Name_Bridge();
				i ++;
			}break;
		case 2:		
			{
				Ret += Name_Bridge();
				Ret += Name_Vowel(); 
				i ++;
			}break;
		}

		Letter_State ++;

		if( Letter_State == 1 )
			Letter_State = rand()%2 + 1;
		else if( Letter_State > 1 )
			Letter_State = 0;
	}

	if( Ret[0] == '-' || Ret[0] == '\'' )
		Ret.erase( Ret.begin() );

	if( Ret[Ret.size()-1] == '-' || Ret[Ret.size()-1] == '\'' )
		Ret.pop_back();

	Ret[0] = toupper( Ret[0] );

	return Ret;
}



Base_AI_Action::Base_AI_Action()
{
	this->ID = 0;
	this->Priority = 0.f;
}

		   
AI_Phenome::AI_Phenome()
{
	this->R = rand()%200 + 55;//54;
	this->G = rand()%200 + 55;//76;
	this->B = rand()%200 + 55;//196;
	this->A = 255;// moving removes invisibility

	u_char Num_Name_Parts = rand()%2 + 1;

	for(u_char i = 0;i < Num_Name_Parts; i++)
		this->Name += Name_Part() + ' ';
}

AI_Brain::AI_Brain()
{
	this->Min_Energy_Before_Eat = 128;// before priority goes to food
	this->Min_Health_Before_Run = 128;// before priority goes to hide
	this->Min_Energy_Before_Heal = 210;
	this->Max_Health_Wanted = 250;

	this->Heal_Amount = 128;
}

AI_Genome::AI_Genome()
{
	this->Speed = 200;// the maximum speed that can be achieved
	this->Strength = 0;
	this->Endurance = 0;
	this->Ferocity = 0;
	
	this->Sight = 0;

	this->Meat_Vs_Veg = 0;// leaning towards % of what can eat
}



AI_Living::AI_Living()
{					   
	this->Health = 100.f;
	this->Energy = 100.f;

	this->Think = true;

	this->Draw_Info = false;

	this->Phys.Size.x = 4;
	this->Phys.Size.y = 4;


	this->Phys.Position.x = 0;
	this->Phys.Position.y = 2;
						
	this->Next_Goal = 0.f;
						  
	this->Bite_Anim = 1.f;
	this->Blink_Anim = -1.f;
	this->Blink_Wait_Anim = 0.f;
}				 

void AI_Living::Consume_Energy( float Amount )
{			   
	float Subtraction = abs(Amount) * ENERGY_CONSUME * (1.f - As_Float(this->m_Genome.Endurance) );
	
	if( this->Energy - Subtraction > 0 )
		this->Energy -= Subtraction;
	else
	{
		this->Energy = 0.f;
		this->Take_Damage( Amount );
	}
}		  					

void AI_Living::Gain_Energy( float Amount )
{			 
	float Subtraction = abs( Amount );
	
	if( this->Energy + Subtraction < 100 )
		this->Energy += Subtraction;
	else
		this->Energy = 100.f;
}

void AI_Living::Take_Damage( float Amount )
{			 
	float Subtraction = abs(Amount) * DAMAGE_RESISTENCE;
	
	if( this->Health - Subtraction > 0 )
		this->Health -= Subtraction;
	else if( this->Health != 0.f )
	{
		this->Reset_Animations();
		this->Draw_Info = false;
		this->Health = 0.f;			  
	}
}

void AI_Living::Gain_Health( float Amount )
{			 
	float Subtraction = abs(Amount);
	
	if( this->Health + Subtraction < 100 )
		this->Health += Subtraction;
	else
		this->Health = 100.f;
}		   

void AI_Living::Heal_With_Energy( float dt )
{
	if( this->Energy > As_Float(this->m_Genome.Brain.Min_Energy_Before_Heal)*100 && this->Health < As_Float(this->m_Genome.Brain.Max_Health_Wanted)*100 )
	{					
		this->Energy -= As_Float(this->m_Genome.Brain.Heal_Amount) * dt * 0.05f;
		this->Health += As_Float(this->m_Genome.Brain.Heal_Amount) * dt * 0.05f;
	}
}


#include "Draw_Utility.h"

u_int Get_Face( u_char Emot )
{
	switch( Emot )
	{													   
	case Emotions::EMOT_DEFAULT:
		return Engine.Resources.Game_Stuff.AI_Eyes_Default;

	case Emotions::EMOT_HAPPY:
		return Engine.Resources.Game_Stuff.AI_Eyes_Happy;

	case Emotions::EMOT_SAD:
		return Engine.Resources.Game_Stuff.AI_Eyes_Sad;

	case Emotions::EMOT_DEAD:
		return Engine.Resources.Game_Stuff.AI_Eyes_Dead;

	default:
		return Engine.Resources.Game_Stuff.AI_Eyes_Default;
	}
}



void AI_Living::Draw()
{			   
	glColor4ub( this->m_Genome.Phenome.R, this->m_Genome.Phenome.G, 
		       this->m_Genome.Phenome.B, this->m_Genome.Phenome.A );

	this->Phys.Draw( true );// draw body

																
	glPushMatrix();
	
	glTranslatef( this->Phys.Position.x + this->Phys.Size.x/2.f, this->Phys.Position.y + this->Phys.Size.y/2.f, 0 );
	glScalef( this->Phys.Size.x/2.f, this->Phys.Size.y/2.f, 1 );


	if( this->Bite_Anim < 1.f )
	{				 
		glPushMatrix();// draw mouth 


		Engine.Resources.Use_Texture( Engine.Resources.Game_Stuff.AI_Mouth );
																	 
		//glTranslatef( 0.5f, 0.5f, 0 );
		glScalef( 1.f-abs(this->Bite_Anim), 1.f-abs(this->Bite_Anim), 1 );

		glBegin( GL_QUADS );					   
			glTexCoord2f(0, 1);glVertex2f( -1, -1 );
			glTexCoord2f(1, 1);glVertex2f( +1, -1 );
			glTexCoord2f(1, 0);glVertex2f( +1, +1 );
			glTexCoord2f(0, 0);glVertex2f( -1, +1 );
		glEnd();

		glPopMatrix();
	}
	

	glPushMatrix();

	if( this->Think && this->Health > 0)
	{
		this->Looking_At = this->Phys.Velocity;

		if( abs(this->Looking_At.x) > 0.5f )
			this->Looking_At.x = RoundToUnit(this->Looking_At.x) * 0.5f;
	
		if( abs(this->Looking_At.y) > 0.5f )
			this->Looking_At.y = RoundToUnit(this->Looking_At.y) * 0.5f;
															 
	}
	Engine.Resources.Use_Texture( Get_Face(this->Facial_Emotion) );

	if( this->Health > 0 )
	{		
		glTranslatef( this->Looking_At.x * 0.5f, this->Looking_At.y * 0.5f, 0 );
		//glTranslatef( 0, 0.5f*abs(this->Blink_Anim), 0 );
		glScalef( 1, abs(this->Blink_Anim), 1 );
	}


	glBegin( GL_QUADS );					   
		glTexCoord2f(0, 1);glVertex2f( -0.8f, -0.8f );
		glTexCoord2f(1, 1);glVertex2f( +0.8f, -0.8f );
		glTexCoord2f(1, 0);glVertex2f( +0.8f, +0.8f );
		glTexCoord2f(0, 0);glVertex2f( -0.8f, +0.8f );
	glEnd();

	glPopMatrix();


	glPopMatrix();

	

	if( this->Draw_Info )			   																								
	{
		glPushMatrix();
		sglDisable( GL_TEXTURE_2D );	

		glTranslatef( this->Phys.Position.x + this->Phys.Size.x/2.f, this->Phys.Position.y + this->Phys.Size.y/2.f, 0 );

		glColor3f( 1, 1, 0 );
		Draw_Quad( -2, -4.25f, this->Energy / 25.f, 0.5f );   

		glColor3f( 1, 0, 0 );
		Draw_Quad( -2, -5.25f, this->Health / 25.f, 0.5f );


		glScalef( 1.f/32.f, 1.f/32.f, 1 );
		Draw_Utility::String_Draw( this->m_Genome.Phenome.Name, -(this->m_Genome.Phenome.Name.length() - 1 )*8, -105, 1,1,1,1 );

		glPopMatrix();
	}
}

void AI_Living::Update( float dt )
{
	this->Phys.Update( dt );

	if( this->Health > 0 )
	{
		if( this->Think )
		{		
			this->Next_Goal -= dt;
			if( this->Next_Goal < 0 )
			{
				this->Next_Goal = GOAL_UPDATE_TIME + rand()%(GOAL_UPDATE_MARGIN * 2) - (float)GOAL_UPDATE_MARGIN;
				this->Plan_Goals();
			}


			this->Next_Check -= dt;
			if( this->Next_Check < 0 )
			{
				this->Next_Check = CHECK_UPDATE_TIME + rand()%(CHECK_UPDATE_MARGIN * 2) - (float)CHECK_UPDATE_MARGIN;
				this->Check_Goals();
			}


			this->Do_Goals( dt );
		}

		this->Animation( dt );

										
		this->Heal_With_Energy( dt );
		this->Consume_Energy( dt * 0.06f );
	}
	else
	{
		this->Facial_Emotion = Emotions::EMOT_DEAD;
	}
}

void AI_Living::Animation( float dt )
{		
	if( this->Energy < 20.f )						
		this->Facial_Emotion = Emotions::EMOT_SAD;
	else if( this->Energy > 80.f )
		this->Facial_Emotion = Emotions::EMOT_HAPPY;
	else
		this->Facial_Emotion = Emotions::EMOT_DEFAULT;


	if( this->Bite_Anim < 1.f )
		this->Bite_Anim += 0.002f * dt;

	
	if( this->Blink_Wait_Anim >= 1.f )
	{
		if( this->Blink_Anim < 1.f )
			this->Blink_Anim += 0.002f * dt;
		else
		{
			this->Blink_Anim = -1.f;
			this->Blink_Wait_Anim = -1.f - rand()%100 / 50.f;
		}
	}
	else
		this->Blink_Wait_Anim += 0.001f * dt;
}

void AI_Living::Reset_Animations()
{			   
	this->Bite_Anim = 1.f;
	this->Blink_Anim = -1.f;
	this->Blink_Wait_Anim = 0.f;

	this->Looking_At = Vector2();
}

Base_AI_Action* AI_Living::Get_Doing_Now()
{
	if( this->Doing_Now.size() > 0 )
		return &this->Doing_Now[this->Doing_Now.size()-1];
	else
		return NULL;
}

float Cur_Priority( AI_Living* This )
{
	Base_AI_Action* Temp = This->Get_Doing_Now();
	if( Temp )
		return Temp->Priority;
	else
		return 0.f;
}

void AI_Living::Plan_Goals()// check current situation
{
	Base_AI_Action nAct;

	// basic necesities go first		 
	if( this->Health / 100.f < As_Float(this->m_Genome.Brain.Min_Health_Before_Run) )// under limit
	{
		if( Cur_Priority( this ) < MAX_PRIORITY )
		{
			// could mean it is under attack or hungry, based on last message

			LC_Object::Object_Base* Closest_Grass = this->Closest_Obj( LC_Object::TYPE_GRASS );
			  
			if( Closest_Grass != NULL )
			{
				nAct.ID = nAct.ACT_MOVE;
				nAct.Params.push_back( (void*)Closest_Grass );	 
				nAct.Priority = MAX_PRIORITY;


				Base_AI_Action Act_Eat;
				Act_Eat.ID = Act_Eat.ACT_EAT; 		   
				Act_Eat.Priority = MAX_PRIORITY;
									   

				this->Doing_Now.push_back( Act_Eat );
				this->Doing_Now.push_back( nAct );
			}
		}
	}
	else if( this->Energy / 100.f < As_Float(this->m_Genome.Brain.Min_Energy_Before_Eat) )// under limit
	{
		if( Cur_Priority( this ) < MAX_PRIORITY )
		{
			LC_Object::Object_Base* Closest_Grass = this->Closest_Obj( LC_Object::TYPE_GRASS );
			  
			if( Closest_Grass != NULL )
			{
				nAct.ID = nAct.ACT_MOVE;
				nAct.Params.push_back( (void*)Closest_Grass );	 
				nAct.Priority = MAX_PRIORITY;


				Base_AI_Action Act_Eat;
				Act_Eat.ID = Act_Eat.ACT_EAT; 		   
				Act_Eat.Priority = MAX_PRIORITY;
									   

				this->Doing_Now.push_back( Act_Eat );
				this->Doing_Now.push_back( nAct );
			}
		}
	}												
}

void AI_Living::Do_Goals( float dt )// do current goal
{
	if( this->Doing_Now.size() > 0 )
	{
		Base_AI_Action* Cur_Act = this->Get_Doing_Now();

		switch( Cur_Act->ID )
		{
		case Base_AI_Action::ACT_NOTHING:break;
		case Base_AI_Action::ACT_MOVE:
			{
				LC_Object::Object_Base* Cur_Obj = (LC_Object::Object_Base*)Cur_Act->Params[0];
				if( Engine.Game.Get_Obj_Index(Cur_Obj) != unsigned(-1) )
					this->Move( RoundToUnit( Cur_Obj->Position.x + Cur_Obj->Size.x/2.f - this->Phys.Position.x - this->Phys.Size.x/2.f ) * As_Float(this->m_Genome.Speed), dt );
				else 
					this->Doing_Now.pop_back();
			}break;      
		case Base_AI_Action::ACT_MOVE_AWAY:break;
		case Base_AI_Action::ACT_JUMP:break;
		case Base_AI_Action::ACT_ATTACK:break;
		case Base_AI_Action::ACT_EAT:
			{	
				if( this->Eat() )
					;
				this->Doing_Now.pop_back();
			}break;


		default:
			//this->Next_Goal = 0.f;
			break;
		}
	}
	else
		;//this->Next_Goal = 0.f;
}

void AI_Living::Check_Goals()// have I succeeded
{	   
	if( this->Doing_Now.size() > 0 )
	{
		Base_AI_Action* Cur_Act = &this->Doing_Now[this->Doing_Now.size()-1];

		switch( Cur_Act->ID )
		{		   
		case Base_AI_Action::ACT_NOTHING:break;
		case Base_AI_Action::ACT_MOVE:
			{			 
				LC_Object::Object_Base* Cur_Obj = (LC_Object::Object_Base*)Cur_Act->Params[0];
				if( Engine.Game.Get_Obj_Index(Cur_Obj) != unsigned(-1) )
				{				 
					LC_Object::Object_Base* Check_Closest = this->Closest_Obj( Cur_Obj->Type );

					if( Cur_Obj != Check_Closest )// new closer grass
						Cur_Act->Params[0] = (void*)Check_Closest;


					if( abs(this->Phys.Position.x + this->Phys.Size.x/2.f - Cur_Obj->Position.x - Cur_Obj->Size.x/2.f) <= CLOSE_ENOUGH )
					{
						this->Doing_Now.pop_back();
					}
				}
				else								
					this->Doing_Now.pop_back();
			}break;      
		case Base_AI_Action::ACT_MOVE_AWAY:break;
		case Base_AI_Action::ACT_JUMP:break;
		case Base_AI_Action::ACT_ATTACK:break;
		case Base_AI_Action::ACT_EAT:break;
		}
	}
}		 

LC_Object::Object_Base* AI_Living::Closest_Obj( u_int Type )// return NULL if no objects in sight
{
	u_int Closest_Obj_Index = -1;
	LC_Object::Object_Base* Ret_Closest = Engine.Game.Get_Obj( Type, &Closest_Obj_Index );

	float Smallest_Distance = Engine.Game.World_W;

	while( Ret_Closest )
	{
		LC_Object::Object_Base* Temp_Obj = Engine.Game.Get_Obj( Type, &Closest_Obj_Index );

		if( Temp_Obj )
		{	 
			float Temp_Dist = abs(this->Phys.Position.x + this->Phys.Size.x/2.f - Temp_Obj->Position.x + Temp_Obj->Size.x/2.f);
			if( Temp_Dist < Smallest_Distance )
			{
				Smallest_Distance = Temp_Dist;
				Ret_Closest = Temp_Obj;
			}
		}
		else
			break;
	}

	return Ret_Closest;
}

void AI_Living::Move( float Dir_X, float dt )
{
	if( this->Health > 0 )
	{
		Dir_X = RoundToUnit( Dir_X );
		Dir_X *= As_Float(this->m_Genome.Speed);

		if( abs(this->Phys.Velocity.x) < As_Float(this->m_Genome.Speed) )
			this->Phys.Apply_Force( Vector2(Dir_X,0), dt );

		this->Consume_Energy( Dir_X * dt * 0.2f );
	}
}

bool AI_Living::Eat()
{
	if( this->Health > 0 )
	{
		for(u_int i = 0;i < Engine.Game.Objects.size(); i++)
		{
			if( Engine.Game.Objects[i].Type = LC_Object::TYPE_GRASS )
			{
				if( abs(this->Phys.Position.x + this->Phys.Size.x/2.f - Engine.Game.Objects[i].Position.x - Engine.Game.Objects[i].Size.x/2.f) < CLOSE_ENOUGH )
				{
					this->Gain_Energy( FOOD_ENERGY );

					Engine.Game.Objects[i].Delete_Flag = true; 

					this->Bite_Anim = -1.f;

					return true;
				}
			}
		}
	}
	
	return false;
}