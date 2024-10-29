#include "MainWindow.h"

MainWindow::MainWindow()
{
	msgCallbacks[WM_CLOSE] = CreateCallback(&MainWindow::HandleClose);
	msgCallbacks[WM_DESTROY] = CreateCallback(&MainWindow::HandleDestroyMsg);
	msgCallbacks[WM_PAINT] = CreateCallback(&MainWindow::HandlePaintMsg);
	
}

void MainWindow::HandleClose(MsgParams& msgParams)
{
	if (MessageBox(hwnd, L"Will you really quiting?", L"Bill Gates's house wont bomb itself!", MB_OKCANCEL) == IDOK)
	{
		DestroyWindow(hwnd);
	}
	return;
}

void MainWindow::HandleDestroyMsg(MsgParams& msgParams)
{
	PostQuitMessage(0);
	return;
}

void MainWindow::HandlePaintMsg(MsgParams& msgParams)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	// PAINTING OCCURS HERE
	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
	// TO HERE
	EndPaint(hwnd, &ps);
	return;
}