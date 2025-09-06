#include "Paintable.h"

HRESULT IPaintable::Create(CComPtr<ID2D1HwndRenderTarget>& pRenderTarget, CComPtr<IDWriteFactory>& pDWriteFactory, CComPtr<ID2D1Factory>& pFactory)
{
	this->pRenderTarget = pRenderTarget;
	this->pDWriteFactory = pDWriteFactory;
	this->pFactory = pFactory;
	return this->CreateOverride();
}
