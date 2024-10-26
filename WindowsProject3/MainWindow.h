#pragma once

#pragma comment(lib, "d2d1")

#include "Tavle.h"
#include <d2d1.h>

class MainWindow : public BaseWindow<MainWindow>
{
public:
	CLASS_NAME_OVERRIDE(MainWindow)

private:
	CComPtr<ID2D1Factory>			pFactory;
	CComPtr<ID2D1HwndRenderTarget>	pRenderTarget;
	CComPtr<ID2D1SolidColorBrush>	pBrush;
	D2D1_ELLIPSE					ellipse;

	void	CalculateLayout();
	HRESULT	CreateGraphicsResources();
	void	DiscardGraphicsResources();

public:
	MainWindow();
	void	HandleDestroy(MsgParams& msgParams);
	void	HandlePaint(MsgParams& msgParams);
	void	HandleSize(MsgParams& msgParams);
	void	HandleCreate(MsgParams& msgParams);

};