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
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0.0f, 0.3);
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
	inline HRESULT OnPaint(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		pRenderTarget->FillEllipse(D2ellipse, pBrush);

		return S_OK;

	}
	// this is not final i may make another virtual for destroying graphics thing idk i need to take a shit
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

	inline HRESULT OnCreate(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		children.emplace_back(std::make_unique<WatchBackground>());

		for (const auto& child : children)
		{
			throw_if_failed(child->OnCreate(pRenderTarget));
		}
	}
	inline HRESULT OnSize(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		for (const auto& child : children)
		{
			throw_if_failed(child->OnSize(pRenderTarget));
		}
	}
	inline HRESULT OnPaint(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		for (const auto& child : children)
		{
			throw_if_failed(child->OnPaint(pRenderTarget));
		}
	}
	inline HRESULT OnDiscard(CComPtr<ID2D1HwndRenderTarget> pRenderTarget) override
	{
		for (const auto& child : children)
		{
			throw_if_failed(child->OnDiscard(pRenderTarget));
		}
	}

};