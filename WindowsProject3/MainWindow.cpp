
#include "MainWindow.h"

MainWindow::MainWindow()
{
	auto clear				= std::make_unique<Clear>();
	auto watchBackground	= std::make_unique<WatchBackground>();
	auto scene				= std::make_unique<Scene>();

	scene->AddChild(std::move(clear));
	scene->AddChild(std::move(watchBackground));

	auto graphicsModule = std::make_unique<GraphicsModule>(&hwnd, std::move(scene));
	modules.emplace_back(std::move(graphicsModule));
}