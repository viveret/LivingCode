#include "source/Engine.h"
S_Engine Engine;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Engine.Start(hInstance);

    MSG msg;

	while(Engine.Is_Running)
    {
		while (PeekMessage(&msg,Engine.Application.Window.hWnd,0,0,PM_REMOVE)>0)// GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage (&msg);
            DispatchMessage (&msg);
        }
		Engine.Main_Loop();
    } 				  

	Engine.Exit();

    return 0;
}