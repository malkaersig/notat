#include "Tavle\include.h"
#include "MainWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE _prevHI, PWSTR pCmdLine, int nCmdShow)
{
	Application<MainWindow> app(
		hInstance, 
		pCmdLine, 
		nCmdShow, 
		L"Notat");

}
