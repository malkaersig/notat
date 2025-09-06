
#include "MainWindow.h"
#include "colors.h"

MainWindow::MainWindow()
{
	bool fileLoaded = false;
	while (!fileLoaded)
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

		pFileManager = std::make_shared<FileManager>(pszFilePath.GetPtr());
		fileLoaded = true;
	}
	catch (_com_error)
	{
		std::cerr << "_com_error (smd idk, go debug)";
		// handle error
		 
	}

	D2D1_RECT_F textRect = {
		0.1, 0.1,
		0.9, 0.9
	};
	auto scene = std::make_shared<Scene>();
	auto clear = std::make_shared<Clear>(myColor5);
	auto text = std::make_shared<LinkedListText>(pFileManager, textRect, 12.0f, myColor1, myColor2);
	auto background = std::make_shared<PaddedRRect>(textRect, 10, myColor4);
	scene->AddChild(clear);
	scene->AddChild(background);
	scene->AddChild(text);
	
	graphicsModule	= std::make_shared<GraphicsModule<MainWindow>>(*this, scene);
	saveFileModule	= std::make_shared<SaveFileModule<MainWindow>>(*this, pFileManager);
	textInputModule = std::make_shared<TextInputModule<MainWindow>>(*this, text);
}