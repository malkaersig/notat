#include "MainWindow.h"

MainWindow::MainWindow()
{
	msgCallbacks[WM_CLOSE] = createCallback(&MainWindow::HandleClose);
	msgCallbacks[WM_DESTROY] = createCallback(&MainWindow::HandleDestroy);
	msgCallbacks[WM_PAINT] = createCallback(&MainWindow::HandlePaint);
	
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