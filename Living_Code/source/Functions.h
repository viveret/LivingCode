#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define _WINSOCKAPI_
#include <Windows.h>
#include <GL\glew.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glext.h>
#include <GL\SOIL.h>
#include <string>
#include <algorithm>

#include "Core.h"

std::string ReadTextFile(std::string Dest);

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);//Handle window messages

void MainLoop();//Controles everything that happens
namespace DrawFunctions
{			 
	void ScreenDraw();//Draws screen
	void GameDraw();//Draws game

    void DrawSky();
    void DrawBullets();
    void DrawAIs();
    void DrawUser();
    void DrawCheckpoints();																	  
    void DrawString(std::string txt,int x,int y,float Scale,float r,float g,float b,float a);
	void DrawString(std::string txt,int x,int y,float r,float g,float b,float a);
    void ShowSplashScreen(std::string Loading);
    void DrawHUD();

    void SwitchTo_3D();
    void SwitchTo_2D();

    void TerrainOps(); 
	void DrawCube(float x,float y,float z,int ID,int MX,int PX,int MY,int PY,int MZ,int PZ,float dist);//Draws cube
}
		
namespace UtilityIO
{  
	void KeyPresses(unsigned int Key);//Get key presses
	void KeyReleases(unsigned char key, int x, int y);//Get key releases
	void SpecialPresses(int key,int x,int y);//Get special key presses
	void SpecialReleases(int key,int x,int y);//Get special key releases

	void MousePresses(int button,int state,int x,int y);//Get mouse presses
	void MouseMovements(int x,int y);//Get mouse movements
	void Mouse_Move();


	GLuint GetTexture(std::string Filename,bool ShowSplash);//Loads images
}

bool SetShaders();

void GameLogic();//Logic for the game
void ScreenLogic();//Logic for the screen

float CollisionX(int dt,float x,float y,float z,float w,float v);//Checks X-Axis collisions
float CollisionY(int dt,float x,float y,float z,float h,float v);//Checks Y-Axis collisions
float CollisionZ(int dt,float x,float y,float z,float w,float v);//Checks Z-Axis collisions


std::string Command(std::string Comm);//Executes command
bool VoidToBool(void* nBool);

std::string CommandPrep(std::string What);
void CheckServerReadys();

bool CheckFlag(int flags,int flag);
									
void memftoa(char* Dest,float* Source);
void mematof(float* Dest,char* Source);
								
std::string To_Str(u_int Value);
std::string To_Str(int Value);

std::string ftostr( float Value );

void replaceAll(std::string& str, const std::string& from, const std::string& to);

#endif // FUNCTIONS_H_INCLUDED