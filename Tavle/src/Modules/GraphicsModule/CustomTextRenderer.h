#pragma once

#include "TavleInclude.h"

/*------------------------------------------------------------------
                       unused but good reference
------------------------------------------------------------------*/

 
class CustomTextRenderer : public IDWriteTextRenderer 
{
private:
	CComPtr<ID2D1Factory> pFactory;
	CComPtr<ID2D1HwndRenderTarget> pRenderTarget;
	CComPtr<ID2D1SolidColorBrush> pBrush;
	UINT cRef = 1;
public:
	CustomTextRenderer(
		CComPtr<ID2D1Factory> pFactory,
		CComPtr<ID2D1HwndRenderTarget> pRenderTarget,
		CComPtr<ID2D1SolidColorBrush> pBrush
		);
	HRESULT QueryInterface(
		REFIID riid,
		LPVOID* ppvObj
	);
	ULONG AddRef();
	ULONG Release();
	HRESULT IsPixelSnappingDisabled(
		void* clientDrawingContext,
		BOOL* isDisabled
	);
	HRESULT GetCurrentTransform(
		void* clientDrawingContext,
		DWRITE_MATRIX* transform
	);
	HRESULT GetPixelsPerDip(
		void* clientDrawingContext,
		FLOAT* pixelsPerDip
	);
	HRESULT DrawGlyphRun(
		void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		DWRITE_MEASURING_MODE measuringMode,
		const DWRITE_GLYPH_RUN* glyphRun,
		const DWRITE_GLYPH_RUN_DESCRIPTION* glyphRunDescription,
		IUnknown* clientDrawingEffect
	);
	HRESULT DrawUnderline(
		void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		DWRITE_UNDERLINE const* underline,
		IUnknown* clientDrawingEffect
	);
	HRESULT DrawStrikethrough(
		void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		DWRITE_STRIKETHROUGH const* strikethrough,
		IUnknown* clientDrawingEffect
	);
	HRESULT DrawInlineObject(
		void* clientDrawingContext,
		FLOAT               originX,
		FLOAT               originY,
		IDWriteInlineObject* inlineObject,
		BOOL                isSideways,
		BOOL                isRightToLeft,
		IUnknown* clientDrawingEffect
	);
};
