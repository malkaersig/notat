#include "CustomTextRenderer.h"

CustomTextRenderer::CustomTextRenderer(CComPtr<ID2D1Factory> pFactory, CComPtr<ID2D1HwndRenderTarget> pRenderTarget, CComPtr<ID2D1SolidColorBrush> pBrush) :
	pFactory(pFactory),
	pRenderTarget(pRenderTarget),
	pBrush(pBrush)
{
}

HRESULT CustomTextRenderer::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	if (!ppvObj)
		return E_INVALIDARG;
	*ppvObj = NULL;
	if (riid == __uuidof(IUnknown))
	{
		*ppvObj = static_cast<IUnknown*>(this);
	}
	else if	(riid == __uuidof(IDWriteTextRenderer))
	{
		*ppvObj = static_cast<IDWriteTextRenderer*>(this);
	}
	else
	{
		*ppvObj = nullptr;
		return E_NOINTERFACE;
	}
	AddRef();
	return S_OK;
}

ULONG CustomTextRenderer::AddRef()
{
	return ++cRef;
}

ULONG CustomTextRenderer::Release()
{
	--cRef;
	if (!cRef)
	{
		delete this;
		return 0;
	}
	return cRef;
}

HRESULT CustomTextRenderer::IsPixelSnappingDisabled(void* clientDrawingContext, BOOL* isDisabled)
{
	*isDisabled = FALSE;
	return S_OK;
}

HRESULT CustomTextRenderer::GetCurrentTransform(void* clientDrawingContext, DWRITE_MATRIX* transform)
{
	pRenderTarget->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
	return S_OK;
}

HRESULT CustomTextRenderer::GetPixelsPerDip(void* clientDrawingContext, FLOAT* pixelsPerDip)
{
	float x, _y;
	pRenderTarget->GetDpi(&x, &_y);
	*pixelsPerDip = x / USER_DEFAULT_SCREEN_DPI;
	return S_OK;
}

HRESULT CustomTextRenderer::DrawGlyphRun(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_MEASURING_MODE measuringMode, const DWRITE_GLYPH_RUN* glyphRun, const DWRITE_GLYPH_RUN_DESCRIPTION* glyphRunDescription, IUnknown* clientDrawingEffect)
{
	try
	{
		// create path geometry
		CComPtr<ID2D1PathGeometry> pPathGeometry;
		throw_if_failed(pFactory->CreatePathGeometry(&pPathGeometry));

		// create sink with open method
		CComPtr<ID2D1GeometrySink> pGeometrySink;
		throw_if_failed(pPathGeometry->Open(&pGeometrySink));

		// get glyphrun outline from direct write and load into glyphrun
		throw_if_failed(glyphRun->fontFace->GetGlyphRunOutline(
			glyphRun->fontEmSize,
			glyphRun->glyphIndices,
			glyphRun->glyphAdvances,
			glyphRun->glyphOffsets,
			glyphRun->glyphCount,
			glyphRun->isSideways,
			glyphRun->bidiLevel % 2,
			pGeometrySink
		));

		// close sink
		throw_if_failed(pGeometrySink->Close());

		// initialize transform matrix
		const D2D1::Matrix3x2F transform_matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);

		// create transformed geometry
		CComPtr<ID2D1TransformedGeometry> pTransformedGeometry;
		throw_if_failed(pFactory->CreateTransformedGeometry(
			pPathGeometry,
			&transform_matrix,
			&pTransformedGeometry
		));
		
		pRenderTarget->FillGeometry(pTransformedGeometry, pBrush);

		return S_OK;
	}
	catch (_com_error)
	{
		return E_FAIL;
	}
}

HRESULT CustomTextRenderer::DrawUnderline(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_UNDERLINE const* underline, IUnknown* clientDrawingEffect)
{
	try
	{
		D2D1_RECT_F rect = {
			0,					underline->offset,
			underline->width,	underline->offset + underline->thickness
		};

		CComPtr<ID2D1RectangleGeometry> pRectangleGeometry;
		throw_if_failed(pFactory->CreateRectangleGeometry(rect, &pRectangleGeometry));
		
		// initialize transform matrix
		const D2D1::Matrix3x2F transform_matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);
		
		// create transformed geometry
		CComPtr<ID2D1TransformedGeometry> pTransformedGeometry;
		throw_if_failed(pFactory->CreateTransformedGeometry(
			pRectangleGeometry,
			&transform_matrix,
			&pTransformedGeometry
		));

		pRenderTarget->FillGeometry(pTransformedGeometry, pBrush);
		
		return S_OK;
	}
	catch (_com_error)
	{
		return E_FAIL;
	}
	
}

HRESULT CustomTextRenderer::DrawStrikethrough(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_STRIKETHROUGH const* strikethrough, IUnknown* clientDrawingEffect)
{
	try
	{
		D2D1_RECT_F rect = {
			0,						strikethrough->offset,
			strikethrough->width,	strikethrough->offset + strikethrough->thickness
		};

		CComPtr<ID2D1RectangleGeometry> pRectangleGeometry;
		throw_if_failed(pFactory->CreateRectangleGeometry(rect, &pRectangleGeometry));

		// initialize transform matrix
		const D2D1::Matrix3x2F transform_matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);

		// create transformed geometry
		CComPtr<ID2D1TransformedGeometry> pTransformedGeometry;
		throw_if_failed(pFactory->CreateTransformedGeometry(
			pRectangleGeometry,
			&transform_matrix,
			&pTransformedGeometry
		));

		pRenderTarget->FillGeometry(pTransformedGeometry, pBrush);

		return S_OK;
	}
	catch (_com_error)
	{
		return E_FAIL;
	}

}

HRESULT CustomTextRenderer::DrawInlineObject(void* clientDrawingContext, FLOAT originX, FLOAT originY, IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft, IUnknown* clientDrawingEffect)
{
	return E_NOTIMPL;
}
