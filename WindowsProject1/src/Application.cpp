#include "Application.h"



Application::Application(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
	:
	hInstance(hInstance),
	pCmdLine(pCmdLine),
	nCmdShow(nCmdShow)
{
	init();
	applicationLoop();
}

int Application::init()
{
	// create window
	if (!mainWindow.Create(
		L"How to rape Noah",
		WS_OVERLAPPEDWINDOW
	))
	{
		std::cerr << "Failed to open window.";
	}

	// SHOW WINDOW
	ShowWindow(mainWindow.Window(), nCmdShow);

	return 0;
}

int Application::applicationLoop()
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
