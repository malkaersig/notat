#pragma once

#include "BaseText.h"

struct SimpleText : public ABaseText
{
private:
	const WCHAR* string;
	UINT32	stringLength;
public:
	SimpleText(const WCHAR* string);
	HRESULT PaintOverride() override;
};
