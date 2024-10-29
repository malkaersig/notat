#pragma once

#include "TavleInclude.h"

struct Paintable
{
	inline virtual HRESULT OnCreate(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) { return S_OK; }
	inline virtual HRESULT OnSize(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) { return S_OK; }
	inline virtual HRESULT OnPaint(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) { return S_OK; }
	inline virtual HRESULT OnDiscard(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) { return S_OK; }
};

struct Clear : public Paintable
{
	inline HRESULT OnPaint(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkBlue));

		return S_OK;
	}
};

struct WatchBackground : public Paintable
{

private:
	D2D1_ELLIPSE					D2ellipse;
	CComPtr<ID2D1SolidColorBrush>	pBrush;

private:
	void CalculateLayout(CComPtr<ID2D1HwndRenderTarget> pRenderTarget)
	{
		if (!pRenderTarget)
		{
			return;
		}
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		float x = size.width / 2;
		float y = size.height / 2;
		const float radius = min(x, y);
		D2ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}

public:
	inline HRESULT OnCreate(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		try
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0.0f);
			throw_if_failed(pRenderTarget->CreateSolidColorBrush(color, &pBrush));
			CalculateLayout(pRenderTarget);
		}
		catch (_com_error)
		{
			return E_FAIL;
		}
		return S_OK;
	}
	inline HRESULT OnSize(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		float x = size.width / 2;
		float y = size.height / 2;
		const float radius = min(x, y);
		D2ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);

		return S_OK;

	}
	// ON PAITN IN GRAPHICS MODULE
	inline HRESULT OnPaint(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		pRenderTarget->FillEllipse(D2ellipse, pBrush);

		return S_OK;

	}
	// ON DISCARD IN GRAPHICSMODULE
	inline HRESULT OnDiscard(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		SafeRelease(&pBrush);

		return S_OK;
	}

};

struct Scene : Paintable
{
private:

	std::vector<std::unique_ptr<Paintable>> children;
public:

	void AddChild(std::unique_ptr<Paintable>&& child)
	{
		children.emplace_back(std::move(child));
	}

	template<typename ... PAINTABLE_PTRS>
	Scene(const PAINTABLE_PTRS&& ... ptrs)
	{
		
	}

	inline HRESULT OnCreate(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		children.emplace_back(std::make_unique<WatchBackground>());

		try
		{
			for (const auto& child : children)
			{
				throw_if_failed(child->OnCreate(pRenderTarget));
			}
		}
		catch (_com_error)
		{
			return E_FAIL;
		}


	}
	inline HRESULT OnSize(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		try
		{
			for (const auto& child : children)
			{
				throw_if_failed(child->OnSize(pRenderTarget));
			}
		}
		catch (_com_error)
		{
			return E_FAIL;
		}
	}
	inline HRESULT OnPaint(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		try
		{
			for (const auto& child : children)
			{
				throw_if_failed(child->OnPaint(pRenderTarget));
			}
		}
		catch (_com_error)
		{
			return E_FAIL;
		}
	}
	inline HRESULT OnDiscard(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		try
		{
			for (const auto& child : children)
			{
				throw_if_failed(child->OnDiscard(pRenderTarget));
			}
		}
		catch (_com_error)
		{
			return E_FAIL;
		}
	}

};

class GraphicsModule : public WindowModule
{

private:
	CComPtr<ID2D1HwndRenderTarget>	pRenderTarget;
	CComPtr<ID2D1Factory>			pFactory;
	HWND* pHwnd;
	std::unique_ptr<Paintable>		paintable;

public:
	GraphicsModule(HWND* pHwnd, std::unique_ptr<Paintable>&& paintable) :
		pHwnd(pHwnd),
		paintable(std::move(paintable))
	{}

	inline void HandleCreateMsg() override
	{
		try
		{
			throw_if_failed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory));
		}
		catch (_com_error)
		{
			return;
		}
	}

	inline void HandleSizeMsg() override
	{
		if (pRenderTarget == NULL)
			return;
		RECT rect;
		GetClientRect(*pHwnd, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		pRenderTarget->Resize(size);
		paintable->OnSize(pRenderTarget);
		InvalidateRect(*pHwnd, NULL, FALSE);
	}

	inline void HandlePaintMsg() override
	{
		try
		{
			throw_if_failed(CreateGraphicsResources());
			PAINTSTRUCT paintStruct;


			BeginPaint(*pHwnd, &paintStruct);
			pRenderTarget->BeginDraw();
			paintable->OnPaint(pRenderTarget);

			// ATTENTION! this error is not accounted for cause FUCK this i hope it fails in the future and i put a shotgun to my mouth over it
			throw_if_failed(pRenderTarget->EndDraw());

		}
		catch (_com_error)
		{
			DiscardGraphicsResources();
			return;
		}
	}

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
			GetClientRect(*pHwnd, &rect);
			D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
			throw_if_failed(pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(*pHwnd, size), &pRenderTarget));
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
		return S_OK;
	}


};