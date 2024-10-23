#include "MainWindow.h"

LRESULT func(MsgParams& msgParams)
{
	int burger = 5;
	auto lambda = [](int value) {};
	return 3;
}

MainWindow::MainWindow()
{
	msgCallbacks[WM_CLOSE] = createCallback(&MainWindow::HandleClose);
	msgCallbacks[WM_DESTROY] = createCallback(&MainWindow::HandleDestroy);
	msgCallbacks[WM_PAINT] = createCallback(&MainWindow::HandlePaint);
	
}

LRESULT MainWindow::HandleMessage(MsgParams& msgParams)
{
	if (msgCallbacks.find(msgParams.uMsg) == msgCallbacks.end())
	{
		return DefWindowProc(m_hwnd, msgParams.uMsg, msgParams.wParam, msgParams.lParam);
	}
	msgCallbacks[msgParams.uMsg](msgParams);
	// if i ever get good at prog and look at this again, please make it so that the lambda functions in
	// msgcallbacks return LRESULT rn i am just happy to have gotten this somewhat working i and dont
	// want to touch this anymore.
	return 0;
}

void MainWindow::HandleClose(MsgParams& msgParams)
{
	if (MessageBox(m_hwnd, L"Will you really quiting?", L"Bill Gates's house wont bomb itself!", MB_OKCANCEL) == IDOK)
	{
		DestroyWindow(m_hwnd);
	}
	return;
}

void MainWindow::HandleDestroy(MsgParams& msgParams)
{
	PostQuitMessage(0);
	return;
}

void MainWindow::HandlePaint(MsgParams& msgParams)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hwnd, &ps);
	// PAINTING OCCURS HERE
	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
	// TO HERE
	EndPaint(m_hwnd, &ps);
	return;
}

MsgCallback MainWindow::createCallback(void(MainWindow::* member)(MsgParams&))
{
	auto lambda = [this, member](MsgParams& msgParams)
		{
			(this->*member)(msgParams);
		};
	return lambda;
}
