#pragma once

#include "TavleInclude.h"
#include <cmath>

struct PaintableCallParams
{
	CComPtr<ID2D1HwndRenderTarget> pRenderTarget;
};

struct Paintable
{
protected:
	CComPtr<ID2D1HwndRenderTarget>	pRenderTarget;
	CComPtr<IDWriteFactory>			pDWriteFactory;
public:
	HRESULT Create(
		CComPtr<ID2D1HwndRenderTarget> pRenderTarget, 
		CComPtr<IDWriteFactory> pDWriteFactory)
	{
		this->pRenderTarget = pRenderTarget;
		this->pDWriteFactory = pDWriteFactory;
		return this->CreateOverride();
	}

	inline virtual HRESULT CreateOverride() { return S_OK; }
	inline virtual HRESULT SizeOverride() { return S_OK; }
	inline virtual HRESULT PaintOverride() { return S_OK; }
	inline virtual HRESULT DiscardOverride() { return S_OK; }
};

struct SimpleText : public Paintable
{
private:
	const WCHAR* string;
	UINT32	stringLength;
	D2D1::ColorF color						= D2D1::ColorF(1.0, 1.0, 1.0);
	CComPtr<ID2D1SolidColorBrush> pBrush	= NULL;
	CComPtr<IDWriteTextFormat> pTextFormat	= NULL;

public:
	SimpleText(const WCHAR* string) :
		string(string),
		stringLength(wcslen(string))
	{}
	inline HRESULT CreateOverride() override
	{
		try
		{
			throw_if_failed(pDWriteFactory->CreateTextFormat(
				L"IMPACT",		// FAMILY NAME
				NULL,				// FONT COLLECTION
				DWRITE_FONT_WEIGHT_REGULAR,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				72.0f,
				L"en-us",
				&pTextFormat
			));
			throw_if_failed(pTextFormat->SetTextAlignment(
				DWRITE_TEXT_ALIGNMENT_CENTER
			));
			throw_if_failed(pTextFormat->SetParagraphAlignment(
				DWRITE_PARAGRAPH_ALIGNMENT_CENTER
			));
			throw_if_failed(pRenderTarget->CreateSolidColorBrush(color, &pBrush));
		}
		catch (_com_error)
		{
			return E_FAIL;
		}
		return S_OK;
	}

	inline HRESULT PaintOverride() override
	{
		if (!pRenderTarget)
		{
			return E_FAIL;
		}
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		D2D1_RECT_F layoutRect = {
			0, 0,
			size.width, size.height
		};
		pRenderTarget->DrawText(string, stringLength, pTextFormat.p, layoutRect, pBrush.p);
		return S_OK;
	}

	// ON DISCARD IN GRAPHICSMODULE
	inline HRESULT DiscardOverride() override
	{
		SafeRelease(&pBrush);
		return S_OK;
	}
};

struct Clear : public Paintable
{
	D2D1::ColorF color = { 0.6, 0.6, 0.0, 1.0 };
	float count = 0;
	inline HRESULT PaintOverride() override
	{
		count = fmod(count + 0.1, 1.0);
		color.b = count;
		pRenderTarget->Clear(color);

		return S_OK;
	}

};

struct WatchBackground : public Paintable
{

private:
	D2D1_ELLIPSE					D2ellipse;
	CComPtr<ID2D1SolidColorBrush>	pBrush;

private:
	void CalculateLayout()
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
	// ON CREATE 
	inline HRESULT CreateOverride() override
	{
		try
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0.0f);
			throw_if_failed(pRenderTarget->CreateSolidColorBrush(color, &pBrush));
			CalculateLayout();
		}
		catch (_com_error)
		{
			return E_FAIL;
		}
		return S_OK;
	}
	inline HRESULT SizeOverride() override
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		float x = size.width / 2;
		float y = size.height / 2;
		const float radius = min(x, y);
		D2ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);

		return S_OK;

	}
	// ON PAITN IN GRAPHICS MODULE
	inline HRESULT PaintOverride() override
	{
		pRenderTarget->FillEllipse(D2ellipse, pBrush);

		return S_OK;

	}
	// ON DISCARD IN GRAPHICSMODULE
	inline HRESULT DiscardOverride() override
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

	inline HRESULT CreateOverride() override
	{

		try
		{
			for (const auto& child : children)
			{
				throw_if_failed(child->Create(pRenderTarget, pDWriteFactory));
			}
		}
		catch (_com_error)
		{
			return E_FAIL;
		}


	}
	inline HRESULT SizeOverride() override
	{
		try
		{
			for (const auto& child : children)
			{
				throw_if_failed(child->SizeOverride());
			}
		}
		catch (_com_error)
		{
			return E_FAIL;
		}
	}
	inline HRESULT PaintOverride() override
	{
		try
		{
			for (const auto& child : children)
			{
				throw_if_failed(child->PaintOverride());
			}
		}
		catch (_com_error)
		{
			return E_FAIL;
		}
	}
	inline HRESULT DiscardOverride() override
	{
		try
		{
			for (const auto& child : children)
			{
				throw_if_failed(child->DiscardOverride());
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
	CComPtr<IDWriteFactory>			pDWriteFactory;
	HWND*							pHwnd;
	std::unique_ptr<Paintable>		paintable;

public:
	GraphicsModule(HWND* pHwnd, std::unique_ptr<Paintable>&& paintable) :
		pHwnd(pHwnd),
		paintable(std::move(paintable))
	{}

	inline void HandleCreateMsg() override
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

	inline void HandleSizeMsg() override
	{
		if (pRenderTarget == NULL)
			return;
		RECT rect;
		GetClientRect(*pHwnd, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		pRenderTarget->Resize(size);
		paintable->SizeOverride();
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
			paintable->PaintOverride();

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
			paintable->Create(pRenderTarget, pDWriteFactory);
		}
		catch (_com_error)
		{
			return E_FAIL;
		}
		return S_OK;
	}

	inline HRESULT DiscardGraphicsResources()
	{
		paintable->DiscardOverride();
		SafeRelease(&pRenderTarget);
		return S_OK;
	}


};