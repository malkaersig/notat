#pragma once

#include "Paintable.h"

struct Clear : public IPaintable
{
private:
	D2D1::ColorF color = { 0.13, 0.11, 0.11, 1.0 };
public:
	HRESULT PaintOverride() override;

};