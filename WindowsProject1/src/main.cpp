



#include "BaseWindow.h"
#include "MainWindow.h"
#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE _hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	const WCHAR* pWindowTitle = L"FUCKING WINDOWWWWWWW!!!";
	Application<MainWindow> app(
		hInstance,
		pCmdLine,
		nCmdShow,
		pWindowTitle
		);
	
	return 0;
}
