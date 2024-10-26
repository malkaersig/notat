
#include "Tavle.h"
#include "MainWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE _prevHI, PWSTR pCmdLine, int nCmdShow)
{
	Application<MainWindow> app(hInstance, pCmdLine, nCmdShow, L"SIPPING ON THE CRACK IM JIZZIN IT");
}