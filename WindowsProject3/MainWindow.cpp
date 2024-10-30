
#include "MainWindow.h"

MainWindow::MainWindow()
{
	auto scene				= std::make_unique<Scene>();
	auto clear				= std::make_unique<Clear>();
	auto watchBackground	= std::make_unique<WatchBackground>();
	auto text				= std::make_unique<SimpleText>(L"JEG KNEPPER BILL GAYTES");

	scene->AddChild(std::move(clear));
	scene->AddChild(std::move(watchBackground));
	scene->AddChild(std::move(text));

	auto graphicsModule = std::make_unique<GraphicsModule>(&hwnd, std::move(scene));
	modules.emplace_back(std::move(graphicsModule));
}