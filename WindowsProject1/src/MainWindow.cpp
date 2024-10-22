#include "MainWindow.h"

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		if (MessageBox(m_hwnd, L"Will you really quiting?", L"Bill Gates's house wont bomb itself!", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(m_hwnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hwnd, &ps);
		// PAINTING OCCURS HERE
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		// TO HERE
		EndPaint(m_hwnd, &ps);
		return 0;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}