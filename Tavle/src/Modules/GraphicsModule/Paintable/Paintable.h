#pragma once

#include "TavleInclude.h"

struct IPaintable
{
protected:
	CComPtr<ID2D1HwndRenderTarget>	pRenderTarget;
	CComPtr<IDWriteFactory>			pDWriteFactory;
	CComPtr<ID2D1Factory>			pFactory;
public:
	HRESULT Create(
		CComPtr<ID2D1HwndRenderTarget>& pRenderTarget,
		CComPtr<IDWriteFactory>& pDWriteFactory,
		CComPtr<ID2D1Factory>& pFactory);

	virtual HRESULT CreateOverride() { return S_OK; }
	virtual HRESULT SizeOverride() { return S_OK; }
	virtual HRESULT PaintOverride() { return S_OK; }
	virtual HRESULT DiscardOverride() { return S_OK; }
};




