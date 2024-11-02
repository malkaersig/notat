
#include "MainWindow.h"


MainWindow::MainWindow()
{
	try
	{
		throw_if_failed(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));

		CComPtr<IFileOpenDialog> pFileOpen;
		throw_if_failed(pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog)));

		throw_if_failed(pFileOpen->Show(NULL));

		CComPtr<IShellItem> pItem;
		throw_if_failed(pFileOpen->GetResult(&pItem));

		ComPtrWrapper<PWSTR> pszFilePath;
		throw_if_failed(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath));

		fm.LoadFile(pszFilePath.GetPtr());
		file_string = fm.GetString();
	}
	catch (_com_error)
	{
		std::cerr << "_com_error (smd idk, go debug)";
		// handle error

	}


	auto scene				= std::make_unique<Scene>();
	auto clear				= std::make_unique<Clear>();
	auto text				= std::make_unique<SimpleText>(file_string.c_str());
	scene->AddChild(std::move(clear));
	scene->AddChild(std::move(text));

	auto graphicsModule = std::make_unique<GraphicsModule>(&hwnd, std::move(scene));
	modules.emplace_back(std::move(graphicsModule));
}