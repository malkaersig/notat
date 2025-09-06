#pragma once
#include "Tavle/include.h"

#define CREATE_COLOR(name, r, g, b) D2D1::ColorF name = D2D1::ColorF((r) / 255.0f, (g) / 255.0f, (b) / 255.0f)

CREATE_COLOR(myColor1, 213, 198, 172);
CREATE_COLOR(myColor2, 150, 121, 90);
CREATE_COLOR(myColor3, 190, 146, 94);
CREATE_COLOR(myColor4, 54, 48, 44);
CREATE_COLOR(myColor5, 46, 33, 34);