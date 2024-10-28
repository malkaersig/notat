#pragma once

#pragma comment(lib, "d2d1")

#include "Tavle.h"
#include "Paintable.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
	CLASS_NAME_OVERRIDE(MainWindow)

private:
	std::vector<std::unique_ptr<WindowModule>> modules;

public:
	MainWindow();
	void	HandleDestroy(MsgParams& msgParams);
	void	HandlePaint(MsgParams& msgParams);
	void	HandleSize(MsgParams& msgParams);
	void	HandleCreate(MsgParams& msgParams);

};

class WindowModule
{
	inline virtual void		HandleDestroy() {}
	inline virtual void		HandlePaint() {}
	inline virtual void		HandleSize() {}
	inline virtual void		HandleCreate() {}
};

class GraphicsModule : public WindowModule
{
private:
	CComPtr<ID2D1HwndRenderTarget>	pRenderTarget;
	CComPtr<ID2D1Factory>			pFactory;
	HWND							m_hwnd;
	std::unique_ptr<Paintable>		paintable;

private:
	inline HRESULT CreateGraphicsResources()
	{
		try
		{
			if (pRenderTarget != NULL)
			{
				return S_OK;
			}
			RECT rect;
			GetClientRect(m_hwnd, &rect);
			D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
			throw_if_failed(pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &pRenderTarget));
			paintable->OnCreate(pRenderTarget);
		}
		catch (_com_error)
		{
			return E_FAIL;
		}
		return S_OK;
	}

	inline HRESULT DiscardGraphicsResources()
	{
		paintable->OnDiscard(pRenderTarget);
		SafeRelease(&pRenderTarget);
	}

	inline void HandleCreate() override
	{
		try
		{
			throw_if_failed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory));
		}
		catch (_com_error)
		{
			return;
		}
	{

	inline void HandleSize() override 
	{
		if (pRenderTarget == NULL)
			return;
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		pRenderTarget->Resize(size);
		paintable->OnSize(pRenderTarget);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}

	inline void HandlePaint() override
	{
		try
		{
			throw_if_failed(CreateGraphicsResources());
			PAINTSTRUCT paintStruct;

			BeginPaint(m_hwnd, &paintStruct);
			pRenderTarget->BeginDraw();
			pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkBlue));
			paintable->OnPaint(pRenderTarget);

			throw_if_failed(pRenderTarget->EndDraw());

		}
		catch (_com_error)
		{
			DiscardGraphicsResources();
			return;
		}
	}



};