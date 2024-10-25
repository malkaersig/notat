#pragma once
#include "BaseWindow.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
	CLASS_NAME_OVERRIDE(MainWindow)

	MainWindow();
	void HandleClose(MsgParams& msgParams);
	void HandleDestroy(MsgParams& msgParams);
	void HandlePaint(MsgParams& msgParams);


};


