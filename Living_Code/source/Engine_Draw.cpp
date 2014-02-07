#include "Engine.h"
extern S_Engine Engine;

#include "Draw_Utility.h"
using namespace Draw_Utility;

			   
#include "Colors.h"



void S_Engine::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glClearColor( 0, 0, 0, 1);

	sglDisable( GL_TEXTURE_2D );

	glBegin( GL_QUADS );		 
		glColor3ub( BGD_R, BGD_G, BGD_B );
		glVertex2f( 0, 0 );
		glVertex2f( ScreenW, 0 );

		glColor3ub( BGU_R, BGU_G, BGU_B );
		glVertex2f( ScreenW, ScreenH );
		glVertex2f( 0, ScreenH );
	glEnd();

	if( this->In_Game )
	{
		this->Game.Draw();

		this->Game.Draw_HUD();
	}



	/*Engine.Resources.Stuff.Use_Texture(Engine.Resources.Items.Item_Icons);//Test_Render);
	glBegin(GL_QUADS);
		glTexCoord2f(0.f,0.f);glVertex2f(300,300);
		glTexCoord2f(1.f,0.f);glVertex2f(300+(float)ScreenW/10.f,300);
		glTexCoord2f(1.f,1.f);glVertex2f(300+(float)ScreenW/10.f,300+(float)ScreenH/10.f);
		glTexCoord2f(0.f,1.f);glVertex2f(300,300+(float)ScreenH/10.f);
	glEnd();		  

	/*this->Resources.Stuff.Use_Texture(this->Resources.Environment.Get_Terrain(4));
	glBegin(GL_QUADS);	   
		glVertex2f(0,0);
		glVertex2f(0,100);
		glVertex2f(100,100);
		glVertex2f(100,0);
	glEnd();			*/
	
	
	//DRAW ENGINE STUFF

	float E_Text_Col = 0.f; // 1.f-this->In_Game

																							 
	std::stringstream Engine_Timer;Engine_Timer<<"Engine Ticks: "<<this->E_Timer.Average;
	String_Draw(Engine_Timer.str(),0.f,ScreenH-20.f,E_Text_Col,E_Text_Col,E_Text_Col,1.f);	

	std::stringstream Logic_Timer;Logic_Timer<<"Logic Ticks: "<<this->Thread_Logic.m_Timer.Average;
	String_Draw(Logic_Timer.str(),0.f,ScreenH-36,1.f,E_Text_Col,E_Text_Col,E_Text_Col,1);
																						 
	std::stringstream Draw_Timer;Draw_Timer<<"Draw Ticks: "<<this->Thread_Draw.m_Timer.Average;
	String_Draw(Draw_Timer.str(),0.f,ScreenH-56,1.f,E_Text_Col,E_Text_Col,E_Text_Col,1);

	std::stringstream Draw_ETC;Draw_ETC<<"Grass: "<< this->Game.Grass_Amount;
	String_Draw(Draw_ETC.str(),0.f,ScreenH-72,1.f,E_Text_Col,E_Text_Col,E_Text_Col,1);

	
	String_Draw("Press 'START' To Toggle Help",ScreenW - 28 * 16 - 20,ScreenH-60,E_Text_Col,E_Text_Col,E_Text_Col,1);
	

	/*if( this->Game.AIS.Player.Ai )
	{
		std::stringstream My_Pos;My_Pos<<"X: "<<this->Game.AIS.Player.Ai->Facing.x<<", Y: "<<this->Game.AIS.Player.Ai->Facing.y;//<<':'<<this->Game.World.Minute<<':'<<this->Game.World.Second;//<<", Z: "<<this->Game.AIS.Player.Ai->Position.z;
		String_Draw(My_Pos.str(),0,256,1.f-this->In_Game,1.f-this->In_Game,1.f-this->In_Game,1);
	}			 */

    glFlush();			
	SwapBuffers(this->Application.Window.hDC);
}

void S_Engine::Draw_Events()
{
	while(this->Event_Stack.size()>0)
	{
		E_Threads::T_Event* nEvent=&this->Event_Stack[0];
		switch(nEvent->ID)
		{
		case E_Threads::Event_Resize:
			{
				this->Application.Window.Reshape(nEvent->Values[0],nEvent->Values[1]);
			}break;
		default:
			{
				std::stringstream Event_Report;
				Event_Report<<"Unkown Thread Draw Event- ID: "<<nEvent->ID;
				this->Register_Error(S_Error(Event_Report.str(),1));
			}break;//unknown event, log it
		}
		//delete event
		nEvent->Destroy();
		this->Event_Stack.erase(this->Event_Stack.begin());
	}
}