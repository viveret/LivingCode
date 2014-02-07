#ifndef WIN_GUI_MAKER
#define WIN_GUI_MAKER

#include <string>
#include <Windows.h>

HMENU Sub_Menu(std::string Name,unsigned int ID);

void Add_Sub(HMENU* Menu,char**Names,unsigned int* IDs,unsigned char Num);

void Make_Menu(HMENU *Menu,HMENU* Sub_Menus, unsigned char Num,std::string Name);


#endif