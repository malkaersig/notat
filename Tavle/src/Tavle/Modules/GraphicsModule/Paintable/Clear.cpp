#include "Clear.h"

Clear::Clear(D2D1::ColorF textColor) :
	textColor(textColor)
{
}

HRESULT Clear::PaintOverride()
{
	pRenderTarget->Clear(textColor);
	return S_OK;
}
