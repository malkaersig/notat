#include "InlineCursor.h"

InlineCursor::InlineCursor(CComPtr<ID2D1HwndRenderTarget> pRenderTarget, CComPtr<ID2D1SolidColorBrush> pBrush, float fontSize) :
	pRenderTarget(pRenderTarget),
	pBrush(pBrush),
	fontSize(fontSize),
	width(fontSize / 10.0f),
	height(fontSize * 1.5f)
{
}

HRESULT InlineCursor::Draw(void* clientDrawingContext, IDWriteTextRenderer* renderer, FLOAT originX, FLOAT originY, BOOL isSideways, BOOL isRightToLeft, IUnknown* clientDrawingEffect)
{
	D2D1_RECT_F cursorRect = {
		originX, originY - fontSize,
		originX + width, originY + height - fontSize
	};
	pRenderTarget->FillRectangle(cursorRect, pBrush);
	return S_OK;
}

HRESULT InlineCursor::GetMetrics(DWRITE_INLINE_OBJECT_METRICS* metrics)
{
	metrics->width = 0;
	metrics->height = 0;
	metrics->baseline = 0;
	metrics->supportsSideways = FALSE;
	return S_OK;
}

HRESULT InlineCursor::GetOverhangMetrics(DWRITE_OVERHANG_METRICS* overhangs)
{
	overhangs->left = 0;
	overhangs->top = 0;
	overhangs->right = 0;
	overhangs->bottom = 0;
	return S_OK;
}

HRESULT InlineCursor::GetBreakConditions(DWRITE_BREAK_CONDITION* breakConditionBefore, DWRITE_BREAK_CONDITION* breakConditionAfter)
{
	*breakConditionBefore = DWRITE_BREAK_CONDITION_NEUTRAL;
	*breakConditionAfter = DWRITE_BREAK_CONDITION_NEUTRAL;
	return S_OK;
}

HRESULT InlineCursor::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	if (!ppvObj)
		return E_INVALIDARG;
	*ppvObj = NULL;
	if (riid == __uuidof(IUnknown))
	{
		*ppvObj = static_cast<IUnknown*>(this);
	}
	else if (riid == __uuidof(IDWriteInlineObject))
	{
		*ppvObj = static_cast<IDWriteInlineObject*>(this);
	}
	else
	{
		*ppvObj = nullptr;
		return E_NOINTERFACE;
	}
	AddRef();
	return S_OK;
}

ULONG InlineCursor::AddRef()
{
	return ++cRef;
}

ULONG InlineCursor::Release()
{
	--cRef;
	if (!cRef)
	{
		delete this;
		return 0;
	}
	return cRef;
}