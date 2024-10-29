#pragma once
#include "BaseWindow.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
	CLASS_NAME_OVERRIDE(MainWindow)

	MainWindow();
	void HandleClose(MsgParams& msgParams);
	void HandleDestroyMsg(MsgParams& msgParams);
	void HandlePaintMsg(MsgParams& msgParams);


};


