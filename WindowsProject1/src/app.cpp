#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct StateInfo {
	BOOL showCloseDialog;
};

inline StateInfo* GetStateInfo(HWND hwnd)
{
	return reinterpret_cast<StateInfo*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE _hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// CREATE AND INITIALIZE STATEINFO STRUCT MEMBERS
	StateInfo* pState = new StateInfo;
	pState->showCloseDialog = TRUE;

	// REGISTER WINDOW CLASS
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = { };


	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// CREATE WINDOW
	HWND hwnd = CreateWindowEx(
		0,							// OPTIONAL WINDOWS STYLES
		CLASS_NAME,					// WINDOW CLASS
		L"Learn to Build a Bomb",	// WINDOW TEXT
		WS_OVERLAPPEDWINDOW,		// WINDOW STYLE

		// SIZE AND POSITION
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,

		NULL,		// PARENT WINDOW
		NULL,		// MENU
		hInstance,	// INSTANCE HANDLE
		pState		// ADDITIONAL APP DATA
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	// SHOW WINDOW
	ShowWindow(hwnd, nCmdShow);

	// RUN MESSAGE LOOP
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	StateInfo* pState;
	if (uMsg == WM_CREATE)
	{
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		pState = reinterpret_cast<StateInfo*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);
	}
	else
	{
		pState = GetStateInfo(hwnd);
	}

	switch (uMsg)
	{
	case WM_CLOSE:
		if (!pState->showCloseDialog)
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		if (MessageBox(hwnd, L"Will you really quiting?", L"Bill Gates's house wont bomb itself!", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// PAINTING OCCURS HERE
		FillRect(hdc, & ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		// TO HERE
		EndPaint(hwnd, &ps);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}