#pragma once
#include "Paintable.h"

struct ABaseText : public IPaintable
{
protected:
	D2D1::ColorF textColor = D2D1::ColorF(1.0, 1.0, 1.0);
	CComPtr<ID2D1SolidColorBrush> pBrush = NULL;
	CComPtr<IDWriteTextFormat> pTextFormat = NULL;

public:
	HRESULT CreateOverride() override;
	HRESULT DiscardOverride() override;

};