
#include "Tavle.h"
#include "MainWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE _prevHI, PWSTR pCmdLine, int nCmdShow)
{
	Application<MainWindow> app(hInstance, pCmdLine, nCmdShow, L"PISSIN IN THE CAT, IM FINGERING IT");
}