#pragma once

#include "BaseText.h"
#include "../GraphicsModule.h"

template <typename WINDOW_TYPE>
struct FpsCounter : public ABaseText
{
private:
	std::weak_ptr<GraphicsModule<WINDOW_TYPE>> wpGraphicsModule;

public:
	FpsCounter(std::weak_ptr<GraphicsModule<WINDOW_TYPE>> wpGraphicsModule) :
		wpGraphicsModule(wpGraphicsModule)
	{}

	HRESULT PaintOverride() override;
};

template<typename WINDOW_TYPE>
HRESULT FpsCounter<WINDOW_TYPE>::PaintOverride()
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



	if (auto pGraphicsModule = wpGraphicsModule.lock())
	{
		// getting fps buffer
		double ms = pGraphicsModule->paint_time;
		ms = ms * 1000;
		wchar_t buffer[8];
		std::swprintf(buffer, 8, L"%.6f", ms);

		// drawing
		pRenderTarget->DrawText(buffer, 8, pTextFormat.p, layoutRect, pBrush.p);
		return S_OK;
	}

	return E_FAIL;

}
