#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#ifndef _UNICODE
#define _UNICODE
#endif 

#include <windows.h>

struct MsgParams
{
	MsgParams(UINT uMsg, WPARAM wParam, LPARAM lParam) :
		uMsg(uMsg),
		wParam(wParam),
		lParam(lParam)
	{}
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
};

template<class DERIVED_TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE* pThis = NULL;
		MsgParams msgParams(uMsg, wParam, lParam);

		if (msgParams.uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)msgParams.lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, (LONG_PTR)pThis);
		}
		if (pThis)
		{
			return pThis->HandleMessage(msgParams);
		}
		else
		{
			return DefWindowProc(hwnd, msgParams.uMsg, msgParams.wParam, msgParams.lParam);
		}
	}

	BaseWindow() :
		m_hwnd(NULL)
	{}

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hwndParent = NULL,
		HMENU hMenu = 0
	)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();

		RegisterClass(&wc);



		m_hwnd = CreateWindowEx(
			dwExStyle,
			ClassName(),
			lpWindowName,
			dwStyle,
			x, y,
			nWidth, nHeight,
			hwndParent,
			hMenu,
			GetModuleHandle(NULL),
			this
		);

		return (m_hwnd ? TRUE : FALSE);
	}

	HWND Window() const
	{
		return m_hwnd;
	}


protected:

	virtual PCWSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(MsgParams& messageParams) = 0;

	HWND m_hwnd;
};