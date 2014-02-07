#ifndef Human_H_INCLUDED
#define Human_H_INCLUDED
						 
#include <Windows.h>
#include <string>
#include <vector>
#include "myMath.h"


#include "Object.h"

							 
#define AI_PHENOME_CHANGE 0.75f
#define AI_GENOME_CHANGE 0.75f
#define AI_BRAIN_CHANGE 0.75f

#define AI_RELATIVE_SPEED 0


#define ENERGY_CONSUME 0.075f * Engine.Game.Harshness
#define DAMAGE_RESISTENCE 0.08f * Engine.Game.Harshness
#define FOOD_ENERGY 45 * (1.f - Engine.Game.Harshness)

														
#define GOAL_UPDATE_TIME 1000 // base difference
#define GOAL_UPDATE_MARGIN 100 // max difference between times

#define CHECK_UPDATE_TIME 200 // base difference
#define CHECK_UPDATE_MARGIN 50 // max difference between times


#define CLOSE_ENOUGH 1.5f


#define AI_MAX_PRIORITY 10.f
#define AI_MED_PRIORITY 0.5f * MAX_PRIORITY
#define AI_LOW_PRIORITY 0.f


namespace AI
{				
	namespace Emotions
	{
		enum {
			EMOT_DEFAULT = 0,
			EMOT_HAPPY,
			EMOT_SAD,
			EMOT_DEAD
		};
	}

	struct Base_AI_Action
	{
		Base_AI_Action();

		u_int ID;
		float Priority;

		std::vector< void* > Params;

		enum{
			ACT_NOTHING = 0,
			ACT_MOVE,     // both are pointers to 
			ACT_MOVE_AWAY,// Vector2 to find the direction to move
			ACT_JUMP,
			ACT_ATTACK,
			ACT_EAT,
			ACT_SET_FACE
		};

	};


	struct AI_Phenome
	{
		AI_Phenome();

		u_char R,G,B,A;// moving removes invisibility
		std::string Name;
	};

	struct AI_Brain
	{	
		AI_Brain();

		u_char Min_Energy_Before_Eat;// before priority goes to food
		u_char Min_Health_Before_Run;// before priority goes to hide
		u_char Min_Energy_Before_Heal;// before using energy to heal
		u_char Max_Health_Wanted;// when it is OK to stop healing

		u_char Heal_Amount;// amount of energy to transfer to health

		u_char Hostility;// how reactive towards others
	};

	struct AI_Genome
	{
		AI_Genome();

		AI_Brain Brain;
		AI_Phenome Phenome;

		u_char Speed;// the maximum that can be achieved
		u_char Strength, Endurance, Ferocity;
		u_char Sight;

		u_char Meat_Vs_Veg;// leaning towards % of what can eat
	};



	struct AI_Living
	{
		AI_Living();

		float Health, Energy;

		void Consume_Energy( float Amount );
		void Gain_Energy( float Amount );
		void Take_Damage( float Amount );
		void Gain_Health( float Amount );
		void Heal_With_Energy( float dt );



		void Draw();
		bool Draw_Info;


		void Update( float dt );

		AI_Genome m_Genome;

		LC_Object::Object_Base Phys;
		Vector2 Looking_At;


		std::vector< int > Messages;
		std::vector< AI_Living* > Remembered_AIs;



		std::vector< Base_AI_Action > Doing_Now;
		Base_AI_Action* Get_Doing_Now();


				  
		bool Think;
		void Plan_Goals();
		void Do_Goals( float dt );
		void Check_Goals();

		float Next_Goal, Next_Check;

		LC_Object::Object_Base* Closest_Obj( u_int Type );// return NULL if no objects in sight



		float Bite_Anim, Blink_Anim, Blink_Wait_Anim;
		u_char Facial_Emotion;
		
		void Animation( float dt );
		void Reset_Animations();// back to zero



		void Move( float X_Dir, float dt );
		void Jump();
		bool Attack( AI_Living* Other );
		bool Eat();// anything that is near
	};
}

#endif // Human_H_INCLUDED
