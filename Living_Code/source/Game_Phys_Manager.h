#ifndef GAME_PHYS_MANAGER
#define GAME_PHYS_MANAGER


namespace Game_Phys_Namespace
{
	struct Game_Phys_Manager
	{
		


		void Refresh();//sync bullet physics to my stuff
		void Refresh_Map();//sync map
		void Refresh_Objs();//sync objects
	};
}


#endif