/*#include "Screen.h"	 
using namespace E_Screen;

#include "Engine.h"
extern S_Engine Engine;	
#include "Draw_Utility.h"
using namespace Draw_Utility;

#define ScreenW Engine.Application.Window.Wind_w
#define ScreenH Engine.Application.Window.Wind_h
	   
void Button_Menu::Draw()
{				
	Engine.Resources.Stuff.Use_Texture(Engine.Resources.GUI.Buttons);
	bool Is_In=Is_In_Box(this->x+ScreenW/2-16,this->y+ScreenH/2-16,32,32,Engine.Application.Input.Mouse_X,Engine.Application.Input.Mouse_Y);
	if(Is_In){glColor3f(0.5,0.5,0.5);}//if in box		
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);glVertex2d(this->x+ScreenW/2-16,this->y+ScreenH/2-16);
		glTexCoord2f(0.5f, 0);glVertex2d(this->x+ScreenW/2+16,this->y+ScreenH/2-16);
		glTexCoord2f(0.5f, 0.5f);glVertex2d(this->x+ScreenW/2+16,this->y+ScreenH/2+16);
		glTexCoord2f(0, 0.5f);glVertex2d(this->x+ScreenW/2-16,this->y+ScreenH/2+16);
	glEnd();  
	String_Draw(this->Display,this->x+ScreenW/2+18,this->y+ScreenH/2,0,0,0,1);
} 

void Button_Var_Bool::Draw()
{								 
	Engine.Resources.Stuff.Use_Texture(Engine.Resources.GUI.Buttons);
	bool Is_In=Is_In_Box(this->x,this->y,32,32,Engine.Application.Input.Mouse_X,Engine.Application.Input.Mouse_Y);
	if(Is_In){glColor3f(0.5,0.5,0.5);}//if in box		
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);glVertex2f(this->x+ScreenW/2.f-16.f,this->y+ScreenH/2.f-16.f);
		glTexCoord2f(0.5f, 0);glVertex2f(this->x+ScreenW/2.f+16.f,this->y+ScreenH/2.f-16.f);
		glTexCoord2f(0.5f, 0.5f);glVertex2f(this->x+ScreenW/2.f+16.f,this->y+ScreenH/2.f+16.f);
		glTexCoord2f(0, 0.5f);glVertex2f(this->x+ScreenW/2.f-16.f,this->y+ScreenH/2.f+16.f);
	glEnd();  
	if(Is_In){glColor3f(1,1,1);}//if in box
}

void Button_Var_Slider::Draw()
{
	//
}

void S_Screen::Draw()
{
	glPushMatrix();

	if(Engine.Resources.Loading.length()==0)//not loading anything
	{
	
		Engine.Resources.Stuff.Use_Texture(Engine.Resources.GUI.Background);
		glBegin(GL_QUADS);
			glTexCoord2f(0.f,1.f);glVertex2f(ScreenW/2-320.0f,ScreenH/2-240.0f);
			glTexCoord2f(1.f,1.f);glVertex2f(ScreenW/2+320.0f,ScreenH/2-240.0f);
			glTexCoord2f(1.f,0.f);glVertex2f(ScreenW/2+320.0f,ScreenH/2+240.0f);
			glTexCoord2f(0.f,0.f);glVertex2f(ScreenW/2-320.0f,ScreenH/2+240.0f);
		glEnd();


		if(false)
		{
			//
		}
		else
		{
			for(unsigned int i=0;i<this->Screen_Images.size();i++)
			{
				Engine.Resources.Stuff.Use_Texture(this->Screen_Images[i].Tex_ID);
				glBegin(GL_QUADS);
					glTexCoord2f(0, 0);glVertex2d(ScreenW/2+this->Screen_Images[i].x-this->Screen_Images[i].w/2,ScreenH/2+this->Screen_Images[i].y-this->Screen_Images[i].h/2);
					glTexCoord2f(1, 0);glVertex2d(ScreenW/2+this->Screen_Images[i].x+this->Screen_Images[i].w/2,ScreenH/2+this->Screen_Images[i].y-this->Screen_Images[i].h/2);
					glTexCoord2f(1, 1);glVertex2d(ScreenW/2+this->Screen_Images[i].x+this->Screen_Images[i].w/2,ScreenH/2+this->Screen_Images[i].y+this->Screen_Images[i].h/2);
					glTexCoord2f(0, 1);glVertex2d(ScreenW/2+this->Screen_Images[i].x-this->Screen_Images[i].w/2,ScreenH/2+this->Screen_Images[i].y+this->Screen_Images[i].h/2);
				glEnd();
			}																
			for(unsigned int b=0;b<this->Screen_Buttons.size();b++)
			{
				this->Screen_Buttons[b]->Draw();	
			}

	/*        if(APP.TypingFor=="client/ip" || APP.TypingFor=="client/port" || APP.TypingFor=="host/port")
			{
				std::string ShowTypingFor="Type ";
				if(APP.TypingFor=="client/ip"){ShowTypingFor+="Client IP";}
				else if(APP.TypingFor=="client/port"){ShowTypingFor+="Client Port";}
				else if(APP.TypingFor=="host/port"){ShowTypingFor+="Port To Host On";}
				else {ShowTypingFor+="ERROR";}
				ShowTypingFor+=": ";
				DrawString(ShowTypingFor,ScreenW/2-(ShowTypingFor.size()*8),ScreenH/2-20,0,0,0,0);
				DrawString(APP.whatTyping,ScreenW/2-(APP.whatTyping.size()*8),ScreenH/2,0,0,0,0);
			}*/				  /*
		}

		//DrawString(APP.InfoToDraw,ScreenW/2-(APP.InfoToDraw.length()/2)*16,ScreenH-20,1,1,1,0);
		//DrawString(APP.STRFPS,0,40,1,1,1,0);
		//std::string showHost="Client";
		//if(Server.Host){showHost="Host";}
		//DrawString(showHost,0,0,1,1,1,0);
		//std::stringstream MousePos;MousePos<<"mX: "<<APP.MouseX<<", mY: "<<APP.MouseY;
		//DrawString(MousePos.str(),0,80,1,1,1,0);
	}
	else//is loading something
	{
		/*Engine.Resources.Stuff.Use_Texture(Engine.Resources.GUI.Splash);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);glVertex2f(ScreenW/2-320,ScreenH/2-240);
            glTexCoord2f(1, 0);glVertex2f(ScreenW/2+320,ScreenH/2-240);
            glTexCoord2f(1, 1);glVertex2f(ScreenW/2+320,ScreenH/2+240);
            glTexCoord2f(0, 1);glVertex2f(ScreenW/2-320,ScreenH/2+240);
        glEnd();

		String_Draw(Engine.Resources.Loading,ScreenW/2-Engine.Resources.Loading.length()*8,ScreenH-22,1,1,1,0);	 */
	/*}

	glPopMatrix();
}			   */