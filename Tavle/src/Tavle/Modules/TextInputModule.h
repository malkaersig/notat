#pragma once
#include "BaseWindow.h"
#include "GraphicsModule/include.h"

template <class WINDOW_TYPE>
class TextInputModule 
{
private:
	WINDOW_TYPE& wnd;
	std::shared_ptr<LinkedListText> pLinkedListText;

public:
	TextInputModule(WINDOW_TYPE& wnd, std::weak_ptr<LinkedListText> pLinkedListText);
	void HandleCharMsg(MsgParams& msgParams);
	void HandleKeyDownMsg(MsgParams& msgParams);
	void HandleLButtonDown(MsgParams& msgParams);
};

template<class WINDOW_TYPE>
inline TextInputModule<WINDOW_TYPE>::TextInputModule(WINDOW_TYPE& wnd, std::weak_ptr<LinkedListText> pLinkedListText) :
	pLinkedListText(pLinkedListText),
	wnd(wnd)
{
	wnd.msgManager.AddFunction<TextInputModule>(this, WM_CHAR, &TextInputModule::HandleCharMsg);
	wnd.msgManager.AddFunction<TextInputModule>(this, WM_KEYDOWN, &TextInputModule::HandleKeyDownMsg);
	wnd.msgManager.AddFunction<TextInputModule>(this, WM_LBUTTONDOWN, &TextInputModule::HandleLButtonDown);

}

template<class WINDOW_TYPE>
inline void TextInputModule<WINDOW_TYPE>::HandleCharMsg(MsgParams& msgParams)
{
	wchar_t c = static_cast<wchar_t>(msgParams.wParam);
	switch (c)
	{
	case L'\b':
	{
		pLinkedListText->Delete();
		break;
	}
	default:
	{
		pLinkedListText->Insert(c);
		break;
	}
	}
	InvalidateRect(wnd.Window(), NULL, false);
}


template<class WINDOW_TYPE>
inline void TextInputModule<WINDOW_TYPE>::HandleKeyDownMsg(MsgParams& msgParams)
{
	switch (msgParams.wParam)
	{
	case VK_RIGHT:
		pLinkedListText->Next();
		InvalidateRect(wnd.Window(), NULL, false);
		break;

	case VK_LEFT:
		pLinkedListText->Prev();
		InvalidateRect(wnd.Window(), NULL, false);
		break;
	}

}

template<class WINDOW_TYPE>
inline void TextInputModule<WINDOW_TYPE>::HandleLButtonDown(MsgParams& msgParams)
{
	int xPos = GET_X_LPARAM(msgParams.lParam);
	int yPos = GET_Y_LPARAM(msgParams.lParam);
	pLinkedListText->GoToScreenPos(xPos, yPos);
}

