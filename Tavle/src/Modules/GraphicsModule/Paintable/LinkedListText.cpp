#include "LinkedListText.h"

HRESULT LinkedListText::CreateOverride()
{

	try
	{
		throw_if_failed(pDWriteFactory->CreateTextFormat(
			L"Arial",		// FAMILY NAME
			NULL,				// FONT COLLECTION
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontSize,
			L"en-us",
			&pTextFormat
		));
		throw_if_failed(pTextFormat->SetTextAlignment(
			DWRITE_TEXT_ALIGNMENT_LEADING
		));
		throw_if_failed(pTextFormat->SetParagraphAlignment(
			DWRITE_PARAGRAPH_ALIGNMENT_NEAR
		));
		throw_if_failed(pRenderTarget->CreateSolidColorBrush(
			color,
			&pBrush
		));
	}
	catch (_com_error)
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT LinkedListText::PaintOverride()
{
	auto pFileManager = wpFileManager.lock();
	if (!pFileManager)
		return E_FAIL;

	if (!pRenderTarget)
	{
		return E_FAIL;
	}

	try
	{
		TextDrawingInfo drawingInfo = pFileManager->CreateTextDrawingInfo();


		// Creating text layout
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		D2D1_RECT_F layoutRect = {
			size.width * windowPosition.left,	size.height * windowPosition.top,
			size.width * windowPosition.right,	size.height * windowPosition.bottom
		};
		const wchar_t* cstr = drawingInfo.string.c_str();
		throw_if_failed(pDWriteFactory->CreateTextLayout(
			cstr,
			wcslen(cstr),
			pTextFormat,
			layoutRect.right - layoutRect.left,
			layoutRect.bottom - layoutRect.top,
			&pTextLayout.p
		));

		D2D1_POINT_2F origin = { layoutRect.left, layoutRect.top };
		pRenderTarget->DrawTextLayout(origin, pTextLayout, pBrush);

		// DRAWING CURSOR
		FLOAT _hitX;
		FLOAT _hitY;
		DWRITE_HIT_TEST_METRICS hitMetrics;
		pTextLayout->HitTestTextPosition(drawingInfo.cursorPos, FALSE, &_hitX, &_hitY, &hitMetrics);

		D2D1_RECT_F hitRect = {
			hitMetrics.left, hitMetrics.top,
			hitMetrics.left + hitMetrics.width, hitMetrics.top + hitMetrics.height
		};

		D2D1_RECT_F cursorRect = {
			hitRect.left + layoutRect.left,
			hitRect.top + layoutRect.top,
			hitRect.left + layoutRect.left + (hitMetrics.height / 8),
			hitRect.bottom + layoutRect.top
		};

		pRenderTarget->FillRectangle(cursorRect, pBrush);


	}
	catch (_com_error)
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT LinkedListText::DiscardOverride()
{
	SafeRelease(&pBrush);
	SafeRelease(&pTextFormat);
	SafeRelease(&pTextLayout);
	return S_OK;
}
void LinkedListText::Insert(const wchar_t& c)
{
	auto pFileManager = wpFileManager.lock();
	if (!pFileManager)
		return;

	pFileManager->Insert(c);
}

void LinkedListText::Delete()
{
	auto pFileManager = wpFileManager.lock();
	if (!pFileManager)
		return;

	pFileManager->Delete();
}

void LinkedListText::Next()
{
	auto pFileManager = wpFileManager.lock();
	if (!pFileManager)
		return;

	pFileManager->Next();
}

void LinkedListText::Prev()
{
	auto pFileManager = wpFileManager.lock();
	if (!pFileManager)
		return;

	pFileManager->Prev();
}

void LinkedListText::GoTo(unsigned int pos)
{
	auto pFileManager = wpFileManager.lock();
	if (!pFileManager)
		return;

	pFileManager->GoTo(pos);
}

void LinkedListText::GoToScreenPos(float x, float y)
{
	if (!(layoutRect.left || layoutRect.top || layoutRect.right || layoutRect.bottom))
	{
		return;
	}
	BOOL isTrailingHit;
	BOOL isInside;
	DWRITE_HIT_TEST_METRICS hitTestMetrics;
	pTextLayout->HitTestPoint(layoutRect.left + x, layoutRect.top + y, &isTrailingHit, &isInside, &hitTestMetrics);
	if (!isInside)
	{
		return;
	}
	GoTo(hitTestMetrics.textPosition);

}