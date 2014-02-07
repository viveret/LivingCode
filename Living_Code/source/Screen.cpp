/*#include "Screen.h"
using namespace E_Screen;

#include "Engine.h"
extern S_Engine Engine;			 
#include "myMath.h"
#include "Draw_Utility.h"
using namespace Draw_Utility;

#include <Windows.h>
#include "GL\\GL.h>
										   
#define ScreenW Engine.Application.Window.Wind_w
#define ScreenH Engine.Application.Window.Wind_h



S_Image::S_Image(int n_x,int n_y,int n_w,int n_h,unsigned int n_Tex_ID)
{
	this->x=n_x;
	this->y=n_y;
	this->w=n_w;
	this->h=n_h;

	this->Tex_ID=n_Tex_ID;
}	

Button_Menu::Button_Menu(int n_x,int n_y,std::string n_Display,std::string n_Go_To)
{
	this->Make(n_x,n_y,n_Display,n_Go_To);
}

Button_Menu::Button_Menu()
{
	this->Make(0,0,std::string(),std::string());
}		   

void Button_Menu::Make(int n_x,int n_y,std::string n_Display,std::string n_Go_To)
{
	this->x=n_x;
	this->y=n_y;
	this->Display=n_Display;
	this->Go_To=n_Go_To;
}

bool Button_Menu::Click()
{
	if(Is_In_Box(this->x+ScreenW/2-16,this->y+ScreenH/2-16,32,32,Engine.Application.Input.Mouse_X,Engine.Application.Input.Mouse_Y))
	{
		Engine.Application.Input.Mouse_L_Down=false;//clicked
		Engine.Screen.Load_Menu(this->Go_To);
		return true;
	}
	return false;
}

void Button_Var_Bool::Make(int n_x,int n_y,std::string n_Display,bool* nValue)
{
	this->x=n_x;
	this->y=n_y;
	this->Display=n_Display;
	this->Value=nValue;
}

Button_Var_Bool::Button_Var_Bool(int n_x,int n_y,std::string n_Display,bool* nValue)
{
	this->Make(n_x,n_y,n_Display,nValue);
}

bool Button_Var_Bool::Click()
{
	//
	return false;
}

void Button_Var_Slider::Make(int n_x,int n_y,std::string n_Display,unsigned int* nValue)
{
	this->x=n_x;
	this->y=n_y;
	this->Display=n_Display;
	this->Value=nValue;
}

Button_Var_Slider::Button_Var_Slider(int n_x,int n_y,std::string n_Display,unsigned int* nValue)
{
	this->Make(n_x,n_y,n_Display,nValue);
}

bool Button_Var_Slider::Click()
{
	return false;
	//
}

//END BUTTONS

void S_Screen::Update(float dt)
{
	if(Engine.Application.Input.Mouse_L_Down)
	{
		for(unsigned int b=0;b<this->Screen_Buttons.size();b++)
		{
			if(this->Screen_Buttons[b]->Click())
			{
				break;
			}
		}
	}
}

void S_Screen::Clear_Menu()
{
	this->Screen_Buttons.clear();
	this->Screen_Images.clear();
}

void S_Screen::Add_Button(Button_Menu nButton)
{
	Button_Menu *Addition = new Button_Menu(nButton);
	this->Screen_Buttons.push_back(Addition);
}

void S_Screen::Add_Button(Button_Var_Bool nButton)
{
	Button_Var_Bool *Addition = new Button_Var_Bool(nButton);
	this->Screen_Buttons.push_back(Addition);
}

void S_Screen::Add_Button(Button_Var_Slider nButton)
{
	Button_Var_Slider *Addition = new Button_Var_Slider(nButton);
	this->Screen_Buttons.push_back(Addition);
}						  */