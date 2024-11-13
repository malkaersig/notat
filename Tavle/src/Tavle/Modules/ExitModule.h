#pragma once

#include "BaseWindow.h"

template<typename WINDOW_TYPE>
class ExitModule
{
public:
	ExitModule(WINDOW_TYPE* pWnd);
	void HandleDestroyMsg(MsgParams& msgParams);
};

template<typename WINDOW_TYPE>
inline ExitModule<WINDOW_TYPE>::ExitModule(WINDOW_TYPE* pWnd)
{
	pWnd->msgManager.AddFunction<ExitModule>(this, WM_DESTROY, &ExitModule::HandleDestroyMsg);
}

template<typename WINDOW_TYPE>
inline void ExitModule<WINDOW_TYPE>::HandleDestroyMsg(MsgParams& msgParams)
{
	PostQuitMessage(0);
}
