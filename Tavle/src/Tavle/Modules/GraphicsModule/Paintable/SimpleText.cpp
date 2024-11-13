#include "SimpleText.h"

SimpleText::SimpleText(const WCHAR* string) :
	string(string),
	stringLength(wcslen(string))
{}

HRESULT SimpleText::PaintOverride()
{
	if (!pRenderTarget)
	{
		return E_FAIL;
	}
	D2D1_SIZE_F size = pRenderTarget->GetSize();
	D2D1_RECT_F layoutRect = {
		0, 0,
		size.width, size.height
	};
	pRenderTarget->DrawText(string, stringLength, pTextFormat.p, layoutRect, pBrush.p);
	return S_OK;
}
