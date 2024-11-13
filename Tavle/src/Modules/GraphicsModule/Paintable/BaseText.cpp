#include "BaseText.h"

HRESULT ABaseText::CreateOverride()
{
	try
	{
		throw_if_failed(pDWriteFactory->CreateTextFormat(
			L"Arial",		// FAMILY NAME
			NULL,				// FONT COLLECTION
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			14.0f,
			L"en-us",
			&pTextFormat
		));
		throw_if_failed(pTextFormat->SetTextAlignment(
			DWRITE_TEXT_ALIGNMENT_LEADING
		));
		throw_if_failed(pTextFormat->SetParagraphAlignment(
			DWRITE_PARAGRAPH_ALIGNMENT_NEAR
		));
		throw_if_failed(pRenderTarget->CreateSolidColorBrush(color, &pBrush));
	}
	catch (_com_error)
	{
		return E_FAIL;
	}
	return S_OK;
}

// ON DISCARD IN GRAPHICSMODULE

HRESULT ABaseText::DiscardOverride()
{
	SafeRelease(&pBrush);
	return S_OK;
}
