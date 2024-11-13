#pragma once
#include "FileManager.h"
#include "Paintable.h"


struct LinkedListText : public IPaintable
{
private:
	D2D1::ColorF color = D2D1::ColorF(1.0, 1.0, 1.0);
	const float fontSize;
	CComPtr<ID2D1SolidColorBrush> pBrush = NULL;
	CComPtr<IDWriteTextFormat> pTextFormat = NULL;
	CComPtr<IDWriteTextLayout> pTextLayout = NULL;
	std::weak_ptr<FileManager> wpFileManager;
	D2D1_RECT_F windowPosition;
	D2D1_RECT_F layoutRect;

public:
	LinkedListText(std::weak_ptr<FileManager> wpFileManager, const D2D1_RECT_F& windowPosition, float fontSize) :
		wpFileManager(wpFileManager),
		windowPosition(windowPosition),
		fontSize(fontSize),
		layoutRect(D2D1_RECT_F {0, 0, 0, 0})
	{}
	HRESULT CreateOverride() override;
	HRESULT PaintOverride() override;
	HRESULT DiscardOverride() override;

	void Insert(const wchar_t& c);
	void Delete();
	void Next();
	void Prev();
	void GoTo(unsigned int pos);
	void GoToScreenPos(float x, float y);
};





