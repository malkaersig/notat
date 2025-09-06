#pragma once
#include "BaseWindow.h"
#include "GraphicsModule/include.h"

template <class WINDOW_TYPE>
class TextInputModule 
{
private:
	WINDOW_TYPE& wnd;
	std::shared_ptr<LinkedListText> pLinkedListText;

	int clickMousePos = -1;
	bool mouseDown = false;

	int selectionBegin = -1;
	int selectionEnd = -1;

private:
	void ClearSelection();

	void Copy();
	void Cut();
	void Paste();

public:
	TextInputModule(WINDOW_TYPE& wnd, std::weak_ptr<LinkedListText> pLinkedListText);
	void HandleCharMsg(MsgParams& msgParams);
	void HandleKeyDownMsg(MsgParams& msgParams);
	void HandleLButtonDown(MsgParams& msgParams);
	void HandleLButtonUp(MsgParams& msgParams);
	void HandleMouseMove(MsgParams& msgParams);

};

template<class WINDOW_TYPE>
inline TextInputModule<WINDOW_TYPE>::TextInputModule(WINDOW_TYPE& wnd, std::weak_ptr<LinkedListText> pLinkedListText) :
	pLinkedListText(pLinkedListText),
	wnd(wnd)
{
	wnd.msgManager.AddFunction<TextInputModule>(this, WM_CHAR, &TextInputModule::HandleCharMsg);
	wnd.msgManager.AddFunction<TextInputModule>(this, WM_KEYDOWN, &TextInputModule::HandleKeyDownMsg);
	wnd.msgManager.AddFunction<TextInputModule>(this, WM_LBUTTONDOWN, &TextInputModule::HandleLButtonDown);
	wnd.msgManager.AddFunction<TextInputModule>(this, WM_LBUTTONUP, &TextInputModule::HandleLButtonUp);
	wnd.msgManager.AddFunction<TextInputModule>(this, WM_MOUSEMOVE, &TextInputModule::HandleMouseMove);


}

template<class WINDOW_TYPE>
inline void TextInputModule<WINDOW_TYPE>::HandleCharMsg(MsgParams& msgParams)
{


	wchar_t c = static_cast<wchar_t>(msgParams.wParam);


	switch (c)
	{
	case L'\b':
	{
		if (selectionBegin < selectionEnd)
		{
			ClearSelection();
			break;
		}
		else
		{
			pLinkedListText->Delete();
			break;
		}

	}
	default:
	{
		bool isValidChar = (iswprint(c) || c == L'\t' || c == L'\n' || c == L'\r');

		if (!isValidChar)
			return;

		ClearSelection();
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
	case 0x43: // C
		if (GetKeyState(VK_CONTROL) & 0x8000)
			Copy();
		break;

	case 0x56: // V
		if (GetKeyState(VK_CONTROL) & 0x8000)
			Paste();
		break;

	case 0x58: // X
		if (GetKeyState(VK_CONTROL) & 0x8000)
			Cut();
		break;

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
	int downX = GET_X_LPARAM(msgParams.lParam);
	int downY = GET_Y_LPARAM(msgParams.lParam);
	clickMousePos = pLinkedListText->GetTextPos(downX, downY).pos;
	mouseDown = true;
	selectionBegin = -1;
	selectionEnd = -1;
	ClearSelection();
}

template<class WINDOW_TYPE>
inline void TextInputModule<WINDOW_TYPE>::HandleLButtonUp(MsgParams& msgParams)
{
	int releaseX = GET_X_LPARAM(msgParams.lParam);
	int releaseY = GET_Y_LPARAM(msgParams.lParam);
	int currentMousePos = pLinkedListText->GetTextPos(releaseX, releaseY).pos;
	if (true
		&& clickMousePos > 0
		&& mouseDown)
	{
		pLinkedListText->GoTo(clickMousePos);
	}
	mouseDown = false;
	InvalidateRect(wnd.Window(), NULL, false);
}

template<class WINDOW_TYPE>
inline void TextInputModule<WINDOW_TYPE>::HandleMouseMove(MsgParams& msgParams)
{
	if (!mouseDown)
		return;
	if (clickMousePos < -1)
		return; 
	int posX = GET_X_LPARAM(msgParams.lParam);
	int posY = GET_Y_LPARAM(msgParams.lParam);
	int currentMousePos = pLinkedListText->GetTextPos(posX, posY).pos;
	if (currentMousePos != clickMousePos)
	{
		if (currentMousePos < clickMousePos)
		{
			selectionBegin = currentMousePos;
			selectionEnd = clickMousePos;
		}
		else
		{
			selectionBegin = clickMousePos;
			selectionEnd = currentMousePos;
		}
		pLinkedListText->SetHightlightText(selectionBegin, selectionEnd - selectionBegin);
		pLinkedListText->GoTo(selectionEnd);

	}
	InvalidateRect(wnd.Window(), NULL, false);
}

template<class WINDOW_TYPE>
inline void TextInputModule<WINDOW_TYPE>::ClearSelection()
{
	pLinkedListText->ResetHighlightingMetricBuffer();
	while (selectionBegin < selectionEnd)
	{
		pLinkedListText->Delete();
		selectionEnd--;
	}
	selectionBegin = -1;
	selectionEnd = -1;
}

template<class WINDOW_TYPE>
inline void TextInputModule<WINDOW_TYPE>::Copy()
{
	LPTSTR lptstr;
	HGLOBAL hglb;

	if (selectionBegin >= selectionEnd)
		return;

	try
	{
		OpenClipboard(wnd.Window());
		EmptyClipboard();
		std::wstring string = pLinkedListText->CreateSubstring(
			selectionBegin, 
			selectionEnd
		);

		hglb = GlobalAlloc(
			GMEM_MOVEABLE,
			(string.size() + 1) * sizeof(wchar_t)
		);
		if (!hglb)
			return;

		lptstr = static_cast<LPTSTR>(GlobalLock(hglb));
		memcpy(lptstr, string.c_str(), string.size() * sizeof(wchar_t));
		lptstr[string.size()] = (wchar_t)0; // NULL CHAR
		GlobalUnlock(hglb);
		SetClipboardData(CF_UNICODETEXT, lptstr);
		CloseClipboard();
	}
	catch (_com_error)
	{
		CloseClipboard();
	}
}

template<class WINDOW_TYPE>
inline void TextInputModule<WINDOW_TYPE>::Cut()
{
	Copy();
	ClearSelection();
	InvalidateRect(wnd.Window(), NULL, FALSE);
}

template<class WINDOW_TYPE>
inline void TextInputModule<WINDOW_TYPE>::Paste()
{
	ClearSelection();

	HGLOBAL hglb;
	LPTSTR lptstr;
	try
	{
		IsClipboardFormatAvailable(CF_UNICODETEXT);
		OpenClipboard(wnd.Window());
		hglb = GetClipboardData(CF_UNICODETEXT);
		if (!hglb)
			return;
		lptstr = static_cast<LPTSTR>(GlobalLock(hglb));
		if (!lptstr)
			return;
		for (wchar_t* p = lptstr; *p != L'\0'; ++p)
		{
			pLinkedListText->Insert(*p);
		}
		CloseClipboard();
		InvalidateRect(wnd.Window(), NULL, false);
	}
	catch (_com_error)
	{
		CloseClipboard();
	}
}
