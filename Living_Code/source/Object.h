#ifndef LIVING_CODE_OBJECT
#define LIVING_CODE_OBJECT

#include "Core.h"

#include "myMath.h"

#define OBJ_BOUNCE 0.1f
#define OBJ_DRAG 0.7f
#define OBJ_GRAVITY -0.03f;
#define OBJ_SPEED 0.02f

/*
#define OBJ_BOUNCE 0.1f
#define OBJ_DRAG 0.065f
#define OBJ_DRAG_RES 1.f
#define OBJ_GRAVITY -0.03f;
#define OBJ_SPEED 0.03f
*/


namespace LC_Object
{
	enum {
		TYPE_UNKNOWN = 0,
		TYPE_GRASS,
		TYPE_ROCK
	};

	struct Object_Base// is an object, not ground -> ie grass, rock, ai
	{
		Object_Base();
		void Delete();
		bool Delete_Flag, Deleted_Draw, Delete_Ready;

		u_int Type, Tex;


		void Draw( bool Used_Color = false );


		void Update( float dt );
		void Apply_Force( Vector2 Force, float dt );



		Vector2 Position, Size, Velocity;

		bool Gravity, Static;
	};



	//
}





#endif