



#include "BaseWindow.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
	PCWSTR ClassName() const { return L"Sample Window Class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE _hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

	MainWindow main;
	if (!main.Create(
		L"How to rape Noah",
		WS_OVERLAPPEDWINDOW
	))
	{
		return 0;
	}

	// SHOW WINDOW
	ShowWindow(main.Window(), nCmdShow);

	// RUN MESSAGE LOOP
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}



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
		FillRect(hdc, & ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		// TO HERE
		EndPaint(m_hwnd, &ps);
		return 0;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}