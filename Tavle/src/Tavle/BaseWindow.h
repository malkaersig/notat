#pragma once

#include <wchar.h>
#include "TavleInclude.h"

#define CLASS_NAME_OVERRIDE(classNameStr) const wchar_t* className = L"" #classNameStr " Custom Class"; PCWSTR ClassName() const override { return className; }

struct MsgParams
{
	MsgParams(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) :
		hwnd(hwnd),
		msg(msg),
		wParam(wParam),
		lParam(lParam)
	{}
	HWND	hwnd;
	UINT	msg;
	WPARAM	wParam;
	LPARAM	lParam;
};

using MsgCallback	= std::function<void(MsgParams&)>;
using MsgType		= UINT;

class MsgProceduresManager
{
private:
	std::unordered_map<MsgType, std::vector<MsgCallback>> msgProcedures;

public:
	template<typename MODULE_TYPE>
	MsgCallback CreateCallback(MODULE_TYPE* owner, void(MODULE_TYPE::* member)(MsgParams&)) const
	{
		std::function<void(MsgParams&)> callback =
			[owner, member](MsgParams& msgParams)
			{
				(owner->*member)(msgParams);
			};
		return callback;
	}

	template<typename MODULE_TYPE>
	void AddFunction(MODULE_TYPE* owner, MsgType&& msg, void(MODULE_TYPE::* member)(MsgParams&))
	{
		auto it = msgProcedures.find(msg);
		if (it == msgProcedures.end())
		{
			msgProcedures[msg] = std::vector<MsgCallback>();
		}
		msgProcedures[msg].emplace_back(CreateCallback<MODULE_TYPE>(owner, member));
	}
	HRESULT Call(MsgParams& msgParams)
	{
		auto it = msgProcedures.find(msgParams.msg);
		if (it == msgProcedures.end())
		{
			return DefWindowProc(msgParams.hwnd, msgParams.msg, msgParams.wParam, msgParams.lParam);
		}
		for (int i = 0; i < msgProcedures[msgParams.msg].size(); ++i)
		{
			msgProcedures[msgParams.msg][i](msgParams);
		}
		return S_OK;
	}
};

template<class DERIVED_TYPE>
class BaseWindow
{
protected:
	float DPIScale = 1.0f;
	HWND hwnd = NULL;

public:
	MsgProceduresManager msgManager;

protected:
	virtual PCWSTR ClassName() const = 0;
	MsgCallback CreateCallback(void(DERIVED_TYPE::* member)(MsgParams&)) const;

public:

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int	x = CW_USEDEFAULT,
		int	y = CW_USEDEFAULT,
		int	nWidth = CW_USEDEFAULT,
		int	nHeight = CW_USEDEFAULT,
		HWND hwndParent = NULL,
		HMENU hMenu = 0
	);
	const HWND& Window() const;
};

template<class DERIVED_TYPE>
inline LRESULT BaseWindow<DERIVED_TYPE>::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DERIVED_TYPE* pThis = NULL;
	MsgParams msgParams(hwnd, msg, wParam, lParam);

	if (msgParams.msg == WM_NCCREATE)
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
		return pThis->msgManager.Call(msgParams);
	}
	else
	{
		return DefWindowProc(hwnd, msgParams.msg, msgParams.wParam, msgParams.lParam);
	}
}

template<class DERIVED_TYPE>
inline BOOL BaseWindow<DERIVED_TYPE>::Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, HWND hwndParent, HMENU hMenu)
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

template<class DERIVED_TYPE>
inline const HWND& BaseWindow<DERIVED_TYPE>::Window() const
{
	return hwnd;
}

template<class DERIVED_TYPE>
inline MsgCallback BaseWindow<DERIVED_TYPE>::CreateCallback(void(DERIVED_TYPE::* member)(MsgParams&)) const
{
	DERIVED_TYPE* pThis = (DERIVED_TYPE*)this;
	return [pThis, member](MsgParams& msgParams)
		{
			(pThis->*member)(msgParams);
		};
}
