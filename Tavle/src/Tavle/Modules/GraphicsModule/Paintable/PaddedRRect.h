#pragma once

#include "Paintable.h"

struct PaddedRRect : public IPaintable
{
private:
	CComPtr<ID2D1SolidColorBrush> pBrush = NULL;
	float paddingAmount;
	D2D1_RECT_F layoutRect;
	D2D1::ColorF textColor;
public:
	
		
	PaddedRRect(D2D1_RECT_F layoutRect, float paddingAmount, D2D1::ColorF textColor = D2D1::ColorF(0.065, 0.075, 0.08, 1.0));

	HRESULT CreateOverride() override;
	HRESULT DiscardOverride() override;
	HRESULT PaintOverride() override;


};