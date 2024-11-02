#pragma once
#include "FileManager.h"

#include "Tavle.h"

class MainWindow : public BaseWindow<MainWindow>
{
private:
	FileManager fm;
	std::wstring file_string;
public:
	CLASS_NAME_OVERRIDE(MainWindow)

public:
	MainWindow();

};



