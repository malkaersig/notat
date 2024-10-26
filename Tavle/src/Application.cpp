#include "Application.h"



Application<class MAIN_WINDOW_TYPE>::Application(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow) :
	hInstance(hInstance),
	pCmdLine((WCHAR*)pCmdLine, [](WCHAR* p) { delete[] p; }),
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

int Application<class MAIN_WINDOW_TYPE>::Init()
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

int Application<class MAIN_WINDOW_TYPE>::ApplicationLoop()
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

int Application<class MAIN_WINDOW_TYPE>::Cleanup()
{
	CoUninitialize();
	return 0;
}
