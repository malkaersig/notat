
#include "MainWindow.h"

MainWindow::MainWindow()
{
	msgCallbacks[WM_CREATE]		= createCallback(&MainWindow::HandleCreate);
	msgCallbacks[WM_DESTROY]	= createCallback(&MainWindow::HandleDestroy);
	msgCallbacks[WM_PAINT]		= createCallback(&MainWindow::HandlePaint);
	msgCallbacks[WM_SIZE]		= createCallback(&MainWindow::HandleSize);

}
void MainWindow::CalculateLayout()
{
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size	= pRenderTarget->GetSize();
		float x				= size.width / 2;
		float y				= size.height / 2;
		const float radius	= min(x, y);
		ellipse				= D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}
}

HRESULT MainWindow::CreateGraphicsResources()
{
	try
	{
		if (pRenderTarget != NULL)
		{
			return S_OK;
		}
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		throw_if_failed(pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &pRenderTarget));
		const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0.0f, 0.3);
		throw_if_failed(pRenderTarget->CreateSolidColorBrush(color, &pBrush));
		CalculateLayout();
	}
	catch (_com_error)
	{
		return E_FAIL;
	}
	return S_OK;
}

void MainWindow::DiscardGraphicsResources()
{
	SafeRelease(&pRenderTarget);
	SafeRelease(&pBrush);
}

void MainWindow::HandleSize(MsgParams& msgParams)
{
	if (pRenderTarget == NULL)
		return;
	RECT rect;
	GetClientRect(m_hwnd, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
	pRenderTarget->Resize(size);
	CalculateLayout();
	InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::HandleDestroy(MsgParams& msgParams)
{
	PostQuitMessage(0);
	return;
}

void MainWindow::HandleCreate(MsgParams& msgParams)
{
	try
	{
		throw_if_failed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory));
	}
	catch (_com_error)
	{
		return;
	}

}

void MainWindow::HandlePaint(MsgParams& msgParams)
{
	try
	{
		throw_if_failed(CreateGraphicsResources());
		PAINTSTRUCT paintStruct;
		
		BeginPaint(m_hwnd, &paintStruct);
		pRenderTarget->BeginDraw();
		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkBlue));
		pRenderTarget->FillEllipse(ellipse, pBrush);

		throw_if_failed(pRenderTarget->EndDraw());

	}
	catch (_com_error)
	{
		DiscardGraphicsResources();
		return;
	}

}