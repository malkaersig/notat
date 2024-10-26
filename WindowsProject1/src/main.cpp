



#include "BaseWindow.h"
#include "MainWindow.h"
#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE _hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Application<MainWindow> app(
		hInstance,
		pCmdLine,
		nCmdShow
		);
	
	return 0;
}
