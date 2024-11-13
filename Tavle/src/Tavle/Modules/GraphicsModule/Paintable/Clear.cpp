#include "Clear.h"

HRESULT Clear::PaintOverride()
{
	pRenderTarget->Clear(color);
	return S_OK;
}
