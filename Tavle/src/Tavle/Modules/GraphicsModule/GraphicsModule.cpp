#include "GraphicsModule.h"


inline void DPIScale::Initialize(HWND hwnd)
{
	float dpi = GetDpiForWindow(hwnd);
	scale = dpi / USER_DEFAULT_SCREEN_DPI;
}
