#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM


#include "Core.h"
#include "myMath.h"

#include <vector>


#define PARTICLE_DRAG 0.1f
#define PARTICLE_BOUNCE 0.5f
#define PARTICLE_SPEED 0.005f


namespace E_Particle_System
{
	struct P_Particle
	{
		P_Particle();
		P_Particle( Vector2 Pos, Vector2 Velo );
		void Update( float dt );

		Vector2 Position;
		Vector2 Velocity;

		float Duration;
	};


	struct P_Particle_System
	{
		P_Particle_System();

		void Update( float dt );
		void Draw();

		bool Running;


		u_int Particles_Per_Sec;
		float Add_Particle;

												  
		u_char Start_R, Start_G, Start_B, Start_A;
		u_char End_R, End_G, End_B, End_A;

		float Start_Size, End_Size;

		Vector2 Position, Velocity;
		float Gravity,Particle_Duration, Rand_Noise;

		std::vector< P_Particle > Particles;


		bool Mem_Used, Mem_Locked;// used == update, locked == draw
	};
}




#endif