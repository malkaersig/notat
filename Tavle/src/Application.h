#pragma once

#include "TavleInclude.h"
#include <iostream>


template<class MAIN_WINDOW_TYPE>
class Application
{
public:
	Application(
		HINSTANCE hInstance,
		PWSTR pCmdLine,
		int nCmdShow);
	int Init();
	int ApplicationLoop();
	int Cleanup();

private:
	HINSTANCE hInstance;
	UNPTR<WCHAR> pCmdLine;
	int nCmdShow;
	MAIN_WINDOW_TYPE mainWindow;
};
