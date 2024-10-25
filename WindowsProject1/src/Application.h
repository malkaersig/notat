#pragma once

#include "MainWindow.h"
#include <iostream>

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
	PWSTR pCmdLine;
	int nCmdShow;
	MainWindow mainWindow;
};
