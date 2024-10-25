#include "Application.h"



Application::Application(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow) :
	hInstance(hInstance),
	pCmdLine(pCmdLine),
	nCmdShow(nCmdShow)
{
	if (Init() < 0)
	{
		Cleanup();
		return;
	}
	ApplicationLoop();
	Cleanup();
	return;
}

int Application::Init()
{
	// Initialize COM
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
	{
		return -1;
	}


	// create window
	if (!mainWindow.Create(
		L"Learn to Build a Bomb",
		WS_OVERLAPPEDWINDOW
	))
	{
		std::cerr << "Failed to open window.";
	}

	// SHOW WINDOW
	ShowWindow(mainWindow.Window(), nCmdShow);

	return 0;
}

int Application::ApplicationLoop()
{
	// RUN MESSAGE LOOP
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

int Application::Cleanup()
{
	CoUninitialize();
	return 0;
}
