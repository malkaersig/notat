#pragma once

#include "TavleInclude.h"
#include <cmath>
#include "FileManager.h"
#include "BaseWindow.h"
#include <chrono>
#include <cwchar>
#include "Paintable/include.h"

#include "CustomTextRenderer.h"

class DPIScale
{
private:
	float scale = 1.0f;
public:
	void Initialize(HWND hwnd);

	template<typename T>
	D2D1_POINT_2F PixelsToDips(T x, T y);
};

template <typename WINDOW_TYPE>
class GraphicsModule
{
private:

	CComPtr<ID2D1HwndRenderTarget>	pRenderTarget;
	CComPtr<ID2D1Factory>			pFactory;
	CComPtr<IDWriteFactory>			pDWriteFactory;
	std::shared_ptr<IPaintable>		paintable;
	float							DPIScale;
	WINDOW_TYPE&					wnd;

public:

	double paint_time = 1000;

public:
	GraphicsModule(WINDOW_TYPE& wnd, std::shared_ptr<IPaintable> paintable);
	//GraphicsModule(std::weak_ptr<WINDOW_TYPE> wpWnd, std::shared_ptr<IPaintable> paintable);

	void HandleCreateMsg(MsgParams& msgParams);
	void HandleSizeMsg(MsgParams& msgParams);
	void HandlePaintMsg(MsgParams& msgParams);

private:
	void InitializeDPIScale();

	template<typename T>
	float PixelsToDips(T pixels);

	inline HRESULT CreateGraphicsResources();
	inline HRESULT DiscardGraphicsResources();


};

template<typename T>
inline D2D1_POINT_2F DPIScale::PixelsToDips(T x, T y)
{
	return D2D1::Point2F(static_cast<float>(x) / scale, static_cast<float>(y) / scale);
}

/*

template<typename WINDOW_TYPE>
inline GraphicsModule<WINDOW_TYPE>::GraphicsModule(std::weak_ptr<WINDOW_TYPE> wpWnd, std::shared_ptr<IPaintable> paintable) :
	wpWnd(wpWnd),
	paintable(paintable)
{
	auto pWnd = wpWnd.lock();
	if (!pWnd)
		return;
	pWnd->msgManager.AddFunction<GraphicsModule<WINDOW_TYPE>>(this, WM_CREATE, &GraphicsModule<WINDOW_TYPE>::HandleCreateMsg);
	pWnd->msgManager.AddFunction<GraphicsModule<WINDOW_TYPE>>(this, WM_PAINT, &GraphicsModule<WINDOW_TYPE>::HandlePaintMsg);
	pWnd->msgManager.AddFunction<GraphicsModule<WINDOW_TYPE>>(this, WM_SIZE, &GraphicsModule<WINDOW_TYPE>::HandleSizeMsg);
}
*/
/*

template<typename WINDOW_TYPE>
inline GraphicsModule<WINDOW_TYPE>::GraphicsModule(WINDOW_TYPE* pWnd, std::shared_ptr<IPaintable> paintable) :
	pWnd(pWnd),
	pHwnd(&(pWnd->Window())),
	paintable(paintable)
{
	pWnd->msgManager.AddFunction<GraphicsModule<WINDOW_TYPE>>(this, WM_CREATE, &GraphicsModule<WINDOW_TYPE>::HandleCreateMsg);
	pWnd->msgManager.AddFunction<GraphicsModule<WINDOW_TYPE>>(this, WM_PAINT, &GraphicsModule<WINDOW_TYPE>::HandlePaintMsg);
	pWnd->msgManager.AddFunction<GraphicsModule<WINDOW_TYPE>>(this, WM_SIZE, &GraphicsModule<WINDOW_TYPE>::HandleSizeMsg);
}
*/

template<typename WINDOW_TYPE>
inline GraphicsModule<WINDOW_TYPE>::GraphicsModule(WINDOW_TYPE& wnd, std::shared_ptr<IPaintable> paintable) :
	wnd(wnd),
	paintable(paintable)
{
	wnd.msgManager.AddFunction<GraphicsModule<WINDOW_TYPE>>(this, WM_CREATE, &GraphicsModule<WINDOW_TYPE>::HandleCreateMsg);
	wnd.msgManager.AddFunction<GraphicsModule<WINDOW_TYPE>>(this, WM_PAINT, &GraphicsModule<WINDOW_TYPE>::HandlePaintMsg);
	wnd.msgManager.AddFunction<GraphicsModule<WINDOW_TYPE>>(this, WM_SIZE, &GraphicsModule<WINDOW_TYPE>::HandleSizeMsg);
}

template<typename WINDOW_TYPE>
inline void GraphicsModule<WINDOW_TYPE>::HandleCreateMsg(MsgParams& msgParams)
{

	// create direct2d factory
	try
	{
		throw_if_failed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory));
	}
	catch (_com_error)
	{
		return;
	}


	// create direct write factory
	try
	{
		throw_if_failed(DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pDWriteFactory)
		));
	}
	catch (_com_error)
	{
		return;
	}


}

template<typename WINDOW_TYPE>
inline void GraphicsModule<WINDOW_TYPE>::HandleSizeMsg(MsgParams& msgParams)
{
	if (pRenderTarget == NULL)
		return;

	RECT rect;
	GetClientRect(wnd.Window(), &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
	pRenderTarget->Resize(size);
	paintable->SizeOverride();
	InvalidateRect(wnd.Window(), NULL, FALSE);

}

template<typename WINDOW_TYPE>
inline void GraphicsModule<WINDOW_TYPE>::HandlePaintMsg(MsgParams& msgParams)
{
	auto paint_msg_start = std::chrono::high_resolution_clock::now();
	try
	{
		throw_if_failed(CreateGraphicsResources());
		PAINTSTRUCT paintStruct;


		BeginPaint(wnd.Window(), &paintStruct);
		pRenderTarget->BeginDraw();
		paintable->PaintOverride();

		// ATTENTION! this error is not accounted for cause FUCK this i hope it fails in the future and i put a shotgun to my mouth over it
		throw_if_failed(pRenderTarget->EndDraw());

	}
	catch (_com_error)
	{
		DiscardGraphicsResources();
		return;
	}
	auto paint_msg_end = std::chrono::high_resolution_clock::now();
	paint_time = std::chrono::duration<double>(paint_msg_end - paint_msg_start).count();
}

template<typename WINDOW_TYPE>
inline void GraphicsModule<WINDOW_TYPE>::InitializeDPIScale()
{
	float dpi = GetDpiForWindow(wnd.Window());
	DPIScale = dpi / USER_DEFAULT_SCREEN_DPI;
}

template<typename WINDOW_TYPE>
inline HRESULT GraphicsModule<WINDOW_TYPE>::CreateGraphicsResources()
{
	try
	{
		if (pRenderTarget != NULL)
		{
			return S_OK;
		}
		RECT rect;
		GetClientRect(wnd.Window(), &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		throw_if_failed(pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(wnd.Window(), size), &pRenderTarget));
		pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
		paintable->Create(pRenderTarget, pDWriteFactory, pFactory);
	}
	catch (_com_error)
	{
		return E_FAIL;
	}
	return S_OK;
}

template<typename WINDOW_TYPE>
inline HRESULT GraphicsModule<WINDOW_TYPE>::DiscardGraphicsResources()
{
	paintable->DiscardOverride();
	SafeRelease(&pRenderTarget);
	return S_OK;
}

template<typename WINDOW_TYPE>
template<typename T>
inline float GraphicsModule<WINDOW_TYPE>::PixelsToDips(T pixels)
{
	return static_cast<float>(pixels) / DPIScale;
}
