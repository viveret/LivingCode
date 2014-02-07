#include "Win32_GUI.h"

HMENU Sub_Menu(std::string Name,unsigned int ID)
{
	HMENU Ret = CreatePopupMenu();
	AppendMenu(Ret, MF_STRING, ID, Name.c_str());

	return Ret;
}

void Add_Sub(HMENU* Menu,char**Names,unsigned int* IDs,unsigned char Num)
{
	for(unsigned char m=0;m<Num;m++)
	{
		AppendMenu(*Menu, MF_STRING, IDs[m], Names[m]);
	}
}

void Make_Menu(HMENU *Menu,HMENU* Sub_Menus, unsigned char Num,std::string Name)
{
	for(unsigned char m=0;m<Num;m++)
	{	 
		AppendMenu(*Menu, MF_STRING | MF_POPUP, (UINT)Sub_Menus[m], Name.c_str());
	}
}