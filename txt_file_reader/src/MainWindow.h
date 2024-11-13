#pragma once
#include "FileManager.h"
#include "Tavle.h"

class MainWindow : public BaseWindow<MainWindow>
{
private:
	std::shared_ptr<MainWindow> pThis;
	
	std::shared_ptr<FileManager> pFileManager;

	std::shared_ptr<GraphicsModule<MainWindow>> graphicsModule;
	std::shared_ptr<SaveFileModule<MainWindow>> saveFileModule;
	std::shared_ptr<TextInputModule<MainWindow>> textInputModule;


public:
	CLASS_NAME_OVERRIDE(MainWindow)

public:
	MainWindow();

};



