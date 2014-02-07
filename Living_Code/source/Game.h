#ifndef ENGINE_GAME
#define ENGINE_GAME

#include "AI.h"
#include "myMath.h"
#include "Particle_System.h"

#include "OpenGL.h"


#define MAX_CLOUD_MOVE  1024.f


namespace E_Game
{
	struct G_Player//player stuff
	{
		G_Player();
		~G_Player();

		void Update(float dt);//update

		AI::AI_Living* Controling, *Highlighting;

		//SETTINGS
		Vector2 Camera_Pos;

		float Help_Show_Anim;
		bool Help_Open;
	};

	struct G_Game
	{
		G_Game();
		~G_Game();
		void Initialize();
		void Destroy();			

		void Draw();	   
		void Draw_HUD();
		float Cloud_Movement;

		void Update(float dt);//logic	


		G_Player Player;


		std::vector< LC_Object::Object_Base > Objects;
		bool Obj_Mem_Lock, Obj_Mem_Used;// used == update, locked == draw


		void Update_Objects( float dt );
		void Delete_Object( LC_Object::Object_Base* Which );

		LC_Object::Object_Base* Get_Obj( u_int Type, u_int* Start_From ); 
		u_int Get_Obj_Index( u_int Type, u_int Start_From = 0 );
		u_int Get_Obj_Index( LC_Object::Object_Base* Obj );


		std::vector< E_Particle_System::P_Particle_System > Particle_Systems;
		std::vector< float > Particle_Life_Times;
		void Update_Particle_Systems( float dt );
		bool Particle_Mem_Used,Particle_Mem_Locked;

											
		std::vector<AI::AI_Living> AIS;
		u_char Max_AIs;


		void Spawn_Grass( float dt );
		float Grass_Duration;
		u_int Grass_Amount;


		unsigned short int World_W, Grass_Max_Amount;
		float Grass_Spawn_Time, Harshness;

	private:
		void Rotate_Perspective();//rotate player perspective
		void Move_Perspective();//move player perspective
		
		//Variables
		//unsigned int Last_Mouse_X,Last_Mouse_Y;

		//E_Objects::Object_Base* Test_Object;

		E_Particle_System::P_Particle_System Test_PSys;
	};
}

#endif