#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <GL\glew.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glext.h>
#include <GL\SOIL.h>

#include <bass.h>
#define MAX_PLAY_TIME 65535

#include "Core.h"

#define ICON_SIZE 16 //16 x 16 icons, 256 in total

namespace E_Resources
{
	typedef unsigned int id;

	/*    DOES NOT EVER CHANGE    */

	struct R_Screen_Resources
	{		 
		R_Screen_Resources();
		void Load_Stuff();

		id Help;
	};

	struct R_Game_Resources
	{			  	 
		R_Game_Resources();
		void Load_Stuff();


		u_int AI_Body, AI_Mouth, AI_Poke;
		id AI_Eyes_Default, AI_Eyes_Happy, AI_Eyes_Sad, AI_Eyes_Dead;

		u_int Shrub;
		u_int Dirt;
		u_int Grass;
		u_int Clouds;


		std::vector<id>Terrain;//texture pack
		id Get_Terrain(id ID);
		//
		id Moon,Sun;
		//skybox
		id Down,Up,Side;
		id Sky_Tex;
	};

	struct R_Resources
	{				
		R_Resources();
		~R_Resources();
								

		id Text, Error;
		R_Screen_Resources Screen_Stuff;
		R_Game_Resources Game_Stuff;

		void Load_Stuff();// load files


		id Load_Texture( std::string Filename, u_int oFlags = 0 );//load file by extension
		id Load_Sound(std::string Filename);//load sound, return id
																  
		void Use_Texture(id ID);//load texture into opengl, return success
		bool Use_Sound(id ID);//play sound	

		GLuint Get_Tex(id ID);
		id New_Tex();
		id Add_Tex(GLuint ID);
																		  
		bool Loaded_Necesities;//for splash screen

		bool Load_Fast;//compressing

		HGLRC hRC;//loading context

	private:

		std::vector<HSAMPLE>Samples;
		std::vector<GLuint>Images;
		
		id Current_Texture;//texture currently binded
	};
}


#endif