#pragma once

#include "TavleInclude.h"

#define CLASS_NAME_OVERRIDE(classNameStr) const wchar_t* className = L"" #classNameStr " Custom Class"; PCWSTR ClassName() const override { return className; }

class WindowModule
{
public:
	inline virtual void	HandleDestroyMsg() {}
	inline virtual void	HandlePaintMsg() {}
	inline virtual void	HandleSizeMsg() {}
	inline virtual void	HandleCreateMsg() {}
};

struct MsgParams
{
	MsgParams(UINT uMsg, WPARAM wParam, LPARAM lParam) :
		uMsg(uMsg),
		wParam(wParam),
		lParam(lParam)
	{}
	UINT	uMsg;
	WPARAM	wParam;
	LPARAM	lParam;
};

using MsgCallback	= std::function<void(MsgParams&)>;
using MsgType		= UINT;

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

			pThis->hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
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
		hwnd(NULL)
	{
		msgCallbacks[WM_CREATE] = CreateCallback(&BaseWindow::HandleCreateMsg);
		msgCallbacks[WM_DESTROY] = CreateCallback(&BaseWindow::HandleDestroyMsg);
		msgCallbacks[WM_PAINT] = CreateCallback(&BaseWindow::HandlePaintMsg);
		msgCallbacks[WM_SIZE] = CreateCallback(&BaseWindow::HandleSizeMsg);
	}

	BOOL Create(
		PCWSTR	lpWindowName,
		DWORD	dwStyle,
		DWORD	dwExStyle		= 0,
		int		x				= CW_USEDEFAULT,
		int		y				= CW_USEDEFAULT,
		int		nWidth			= CW_USEDEFAULT,
		int		nHeight			= CW_USEDEFAULT,
		HWND	hwndParent		= NULL,
		HMENU	hMenu			= 0
	)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();

		RegisterClass(&wc);



		hwnd = CreateWindowEx(
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

		return (hwnd ? TRUE : FALSE);
	}

	HWND Window() const
	{
		return hwnd;
	}


protected:
	virtual PCWSTR ClassName() const = 0;

	LRESULT HandleMessage(MsgParams& msgParams)
	{
		auto it = msgCallbacks.find(msgParams.uMsg);
		if (it != msgCallbacks.end())
		{
			msgCallbacks[msgParams.uMsg](msgParams);
			return 0;
		}
		return DefWindowProc(hwnd, msgParams.uMsg, msgParams.wParam, msgParams.lParam);
	}

	MsgCallback CreateCallback(void(DERIVED_TYPE::* member)(MsgParams&))
	{
		DERIVED_TYPE* pThis = (DERIVED_TYPE*)this;
		return [pThis, member](MsgParams& msgParams)
			{
				(pThis->*member)(msgParams);
			};
	}

	void InitializeDPIScale()
	{
		float dpi = GetDpiForWindow(hwnd);
		g_DPIScale = dpi / USER_DEFAULT_SCREEN_DPI;
	}

	template<typename T>
	float PixelsToDips(T pixels)
	{
		return static_cast<float>(pixels) / g_DPIScale;
	}


	void HandleDestroyMsg(MsgParams& msgParams)
	{
		for (const auto& module : modules)
		{
			module->HandleDestroyMsg();
		}
		PostQuitMessage(0);
		return;
	}
	void HandlePaintMsg(MsgParams& msgParams)
	{
		for (const auto& module : modules)
		{
			module->HandlePaintMsg();
		}
	}
	void HandleSizeMsg(MsgParams& msgParams)
	{
		for (const auto& module : modules)
		{
			module->HandleSizeMsg();
		}
	}
	void HandleCreateMsg(MsgParams& msgParams)
	{
		for (const auto& module : modules)
		{
			module->HandleCreateMsg();
		}
	}

protected:
	float g_DPIScale = 1.0f;
	std::unordered_map<MsgType, MsgCallback> msgCallbacks;
	HWND hwnd;
	std::vector<std::unique_ptr<WindowModule>> modules;

};
