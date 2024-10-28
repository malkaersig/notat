
#include "MainWindow.h"

MainWindow::MainWindow()
{
	msgCallbacks[WM_CREATE]		= createCallback(&MainWindow::HandleCreate);
	msgCallbacks[WM_DESTROY]	= createCallback(&MainWindow::HandleDestroy);
	msgCallbacks[WM_PAINT]		= createCallback(&MainWindow::HandlePaint);
	msgCallbacks[WM_SIZE]		= createCallback(&MainWindow::HandleSize);

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
		scene.OnCreate(pRenderTarget); 
	}
	catch (_com_error)
	{
		return E_FAIL;
	}
	return S_OK;

}

void MainWindow::DiscardGraphicsResources()
{
	scene.OnDiscard(pRenderTarget);
	SafeRelease(&pRenderTarget);
}

void MainWindow::HandleSize(MsgParams& msgParams)
{

}

void MainWindow::HandleDestroy(MsgParams& msgParams)
{
	PostQuitMessage(0);
	return;
}

void MainWindow::HandleCreate(MsgParams& msgParams)
{


}

void MainWindow::HandlePaint(MsgParams& msgParams)
{


}