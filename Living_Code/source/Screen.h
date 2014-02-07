/*#ifndef ENGINE_SCREEN
#define ENGINE_SCREEN

#include <string>
#include <vector>

namespace E_Screen
{
	struct Button_Base//regular button
	{
		int x,y;
		std::string Display;//what it shows
		virtual bool Click()=0;//check if mouse is down in that area
		virtual void Draw()=0;
	};
	struct Button_Menu:public Button_Base
	{								  
		void Make(int n_x,int n_y,std::string n_Display,std::string n_Go_to);
		Button_Menu(int n_x,int n_y,std::string n_Display,std::string n_Go_to);
		Button_Menu();

		std::string Go_To;//go to is the next menu object it will go to, back = go back
		bool Click();
		void Draw();
	};										 
	struct Button_Var_Bool:public Button_Base
	{
		bool* Value;
		bool Click();
		void Make(int n_x,int n_y,std::string n_Display,bool* nValue);
		Button_Var_Bool(int n_x,int n_y,std::string n_Display,bool* nValue);
		void Draw();
	};
	struct Button_Var_Slider:public Button_Base
	{
		unsigned int* Value;
		bool Click();
		void Make(int n_x,int n_y,std::string n_Display,unsigned int* nValue);
		Button_Var_Slider(int n_x,int n_y,std::string n_Display,unsigned int* nValue);
		void Draw();
	};

	struct S_Image//screen image
	{
		S_Image(int n_x,int n_y,int n_w,int n_h,unsigned int n_Tex_ID);
		int x,y,w,h;//size
		unsigned int Tex_ID;//texture
	};

	struct S_Modual//base window
	{
		//
	};
	//struct S_Log
	struct S_Screen
	{
		std::vector<std::string> Menu_Trace;

		std::vector<S_Image> Screen_Images;//images on screen
		std::vector<Button_Base*> Screen_Buttons;//buttons on screen
		void Draw();
		void Update(float dt);

		void Load_Menu(std::string Menu);
		void Clear_Menu();
		//add buttons of all kinds	  
		void Add_Button(Button_Menu nButton);
		void Add_Button(Button_Var_Bool nButton);
		void Add_Button(Button_Var_Slider nButton);
	};
}

#endif													   */