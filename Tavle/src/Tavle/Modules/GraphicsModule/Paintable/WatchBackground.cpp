#include "WatchBackground.h"

inline void WatchBackground::CalculateLayout()
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

// ON CREATE 

inline HRESULT WatchBackground::CreateOverride()
{
	try
	{
		const D2D1_COLOR_F textColor = D2D1::ColorF(1.0f, 0.95f, 0.9f);
		throw_if_failed(pRenderTarget->CreateSolidColorBrush(textColor, &pBrush));
		CalculateLayout();
	}
	catch (_com_error)
	{
		return E_FAIL;
	}
	return S_OK;
}

inline HRESULT WatchBackground::SizeOverride()
{
	D2D1_SIZE_F size = pRenderTarget->GetSize();
	float x = size.width / 2;
	float y = size.height / 2;
	const float radius = min(x, y);
	D2ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);

	return S_OK;

}

// ON PAITN IN GRAPHICS MODULE

inline HRESULT WatchBackground::PaintOverride()
{
	pRenderTarget->FillEllipse(D2ellipse, pBrush);

	return S_OK;

}

// ON DISCARD IN GRAPHICSMODULE

inline HRESULT WatchBackground::DiscardOverride()
{
	SafeRelease(&pBrush);

	return S_OK;
}
