#pragma once

#include "Paintable.h"

struct WatchBackground : public IPaintable
{

private:
	D2D1_ELLIPSE					D2ellipse;
	CComPtr<ID2D1SolidColorBrush>	pBrush;

private:
	void CalculateLayout();

public:
	inline HRESULT CreateOverride() override;
	inline HRESULT SizeOverride() override;
	inline HRESULT PaintOverride() override;
	inline HRESULT DiscardOverride() override;

};
