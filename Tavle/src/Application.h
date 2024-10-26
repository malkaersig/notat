#pragma once

#include "COM/Helpers.h"
#include "BaseWindow.h"
#include <iostream>


template<typename MAIN_WINDOW_TYPE>
class Application
{
public:
	Application(
		HINSTANCE hInstance,
		PCWSTR pCmdLine,
		int nCmdShow,
		PCWSTR pWindowTitle);

	int	Init();
	int	ApplicationLoop();
	int	Cleanup();

private:
	HINSTANCE			hInstance;
	PCWSTR				pCmdLine;
	PCWSTR				pWindowTitle;
	int					nCmdShow;
	MAIN_WINDOW_TYPE	mainWindow;
};

template<typename MAIN_WINDOW_TYPE>
Application<MAIN_WINDOW_TYPE>::Application(HINSTANCE hInstance, PCWSTR pCmdLine, int nCmdShow, PCWSTR pWindowTitle) :
	hInstance(hInstance),
	pCmdLine(pCmdLine),
	nCmdShow(nCmdShow),
	pWindowTitle(pWindowTitle)
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

template<typename MAIN_WINDOW_TYPE>
int Application<MAIN_WINDOW_TYPE>::Init()
{
	// Initialize COM
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
	{
		return -1;
	}


	// create window
	if (!mainWindow.Create(
		pWindowTitle,
		WS_OVERLAPPEDWINDOW
	))
	{
		std::cerr << "Failed to open window.";
	}

	// SHOW WINDOW
	ShowWindow(mainWindow.Window(), nCmdShow);

	return 0;
}

template<typename MAIN_WINDOW_TYPE>
int Application<MAIN_WINDOW_TYPE>::ApplicationLoop()
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

template<typename MAIN_WINDOW_TYPE>
int Application<MAIN_WINDOW_TYPE>::Cleanup()
{
	CoUninitialize();
	return 0;
}
