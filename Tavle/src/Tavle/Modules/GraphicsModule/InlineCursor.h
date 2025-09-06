#pragma once

/*------------------------------------------------------------------
					   unused but good reference
------------------------------------------------------------------*/

#include "Tavle\TavleInclude.h"

class InlineCursor : public IDWriteInlineObject
{
private:
	float width;
	float height;
	float fontSize;
	CComPtr<ID2D1HwndRenderTarget> pRenderTarget;
	CComPtr<ID2D1SolidColorBrush> pBrush;
	int cRef;
public:
	InlineCursor(
		CComPtr<ID2D1HwndRenderTarget> pRenderTarget, 
		CComPtr<ID2D1SolidColorBrush> pBrush,
		float fontSize
	);
	HRESULT Draw(
		void* clientDrawingContext,
		IDWriteTextRenderer* renderer,
		FLOAT originX,
		FLOAT originY,
		BOOL isSideways,
		BOOL isRightToLeft,
		IUnknown* clientDrawingEffect
	);
	HRESULT GetMetrics(
		DWRITE_INLINE_OBJECT_METRICS* metrics
	);
	HRESULT GetOverhangMetrics(
		DWRITE_OVERHANG_METRICS* overhans
	);
	HRESULT GetBreakConditions(
		DWRITE_BREAK_CONDITION* breakConditionBefore,
		DWRITE_BREAK_CONDITION* breakConditionAfter
	);
	HRESULT QueryInterface(
		REFIID riid,
		LPVOID* ppvObj
	);
	ULONG AddRef();
	ULONG Release();
};