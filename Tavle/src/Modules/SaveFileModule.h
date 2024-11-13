#pragma once

#include "BaseWindow.h"
#include "FileManager.h"

template<typename WINDOW_TYPE>
class SaveFileModule
{
private:
	std::weak_ptr<FileManager> wpFileManager;
	WINDOW_TYPE& wnd;

public:
	SaveFileModule(WINDOW_TYPE& wnd, std::weak_ptr<FileManager> wpFileManager);
	void HandleDestroyMsg(MsgParams& msgParams);
};

template<typename WINDOW_TYPE>
inline SaveFileModule<WINDOW_TYPE>::SaveFileModule(WINDOW_TYPE& wnd, std::weak_ptr<FileManager> wpFileManager) :
	wpFileManager(wpFileManager),
	wnd(wnd)
{
	wnd.msgManager.AddFunction<SaveFileModule>(this, WM_DESTROY, &SaveFileModule::HandleDestroyMsg);
}

template<typename WINDOW_TYPE>
inline void SaveFileModule<WINDOW_TYPE>::HandleDestroyMsg(MsgParams& msgParams)
{
	auto pFileManager = wpFileManager.lock();
	if (pFileManager)
	{
		std::wstringstream message;
		message << L"Save file " << pFileManager->Filepath() << "?";
		if (MessageBox(wnd.Window(), message.str().c_str(), L"Save File", MB_YESNO) == IDYES)
		{
			pFileManager->Save();
		}
	}
	PostQuitMessage(0);
}
