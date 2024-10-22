#pragma once

#include "MainWindow.h"
#include <iostream>

#define MAGNUS_SUCCESS 2
#define FAILED_TO_OPEN_WINDOW 501

class Application
{
public:
	Application(
		HINSTANCE hInstance,
		PWSTR pCmdLine,
		int nCmdShow);
	int init();
	int applicationLoop();

private:
	HINSTANCE hInstance;
	PWSTR pCmdLine;
	int nCmdShow;
	MainWindow mainWindow;
};
