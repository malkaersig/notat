#include "PaddedRRect.h"

PaddedRRect::PaddedRRect(D2D1_RECT_F layoutRect, float paddingAmount, D2D1::ColorF textColor) :
	layoutRect(layoutRect),
	paddingAmount(paddingAmount),
	textColor(textColor)
{
}

HRESULT PaddedRRect::CreateOverride()
{
	try
	{
		throw_if_failed(pRenderTarget->CreateSolidColorBrush(textColor, &pBrush));
	}
	catch (_com_error)
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT PaddedRRect::DiscardOverride()
{
	SafeRelease(&pBrush);
	return S_OK;
}

HRESULT PaddedRRect::PaintOverride()
{
	// Creating text layout
	D2D1_SIZE_F size = pRenderTarget->GetSize();
	D2D1_RECT_F renderRect = {
		size.width * layoutRect.left,	size.height * layoutRect.top,
		size.width * layoutRect.right,	size.height * layoutRect.bottom
	};
	renderRect = {
		renderRect.left - paddingAmount, renderRect.top - paddingAmount,
		renderRect.right + paddingAmount, renderRect.bottom + paddingAmount
	};
	D2D1_ROUNDED_RECT roundedRect = {
		renderRect,
		paddingAmount,
		paddingAmount
	};
	pRenderTarget->FillRoundedRectangle(roundedRect, pBrush);
	return S_OK;
}
