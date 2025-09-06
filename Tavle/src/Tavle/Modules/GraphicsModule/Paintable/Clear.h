#pragma once

#include "Paintable.h"

struct Clear : public IPaintable
{
private:
	D2D1::ColorF textColor;
public:
	Clear(D2D1::ColorF textColor = D2D1::ColorF(0.045, 0.05, 0.065, 1.0));

	HRESULT PaintOverride() override;

};