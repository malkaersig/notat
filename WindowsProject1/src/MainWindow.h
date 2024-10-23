#pragma once
#include "BaseWindow.h"

#include <unordered_map>
#include <functional>

using MsgCallback = std::function<void(MsgParams&)>;
using MsgType = UINT;


class MainWindow : public BaseWindow<MainWindow>
{
public:
	MainWindow();

	PCWSTR ClassName() const override { return className; }
	LRESULT HandleMessage(MsgParams& msgParams) override;
	void HandleClose(MsgParams& msgParams);
	void HandleDestroy(MsgParams& msgParams);
	void HandlePaint(MsgParams& msgParams);

private:

	MsgCallback createCallback(void(MainWindow::* member)(MsgParams&));
	const wchar_t* className = L"Sample Window Class";
	std::unordered_map<MsgType, MsgCallback> msgCallbacks;
};


