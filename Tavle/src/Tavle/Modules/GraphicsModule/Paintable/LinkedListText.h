#pragma once
#include "Tavle/FileManager.h"
#include "Paintable.h"
#include "array"

struct MouseCursorPos
{
public:
	MouseCursorPos(int pos = -1, bool isTrailing = false, bool isValid = false) :
		pos(pos),
		isTrailing(isTrailing),
		isValid(isValid)
	{}
	int pos;
	bool isTrailing;
	bool isValid;
};


struct LinkedListText : public IPaintable
{
private:
	D2D1::ColorF textColor;
	D2D1::ColorF selectionColor;
	const float fontSize;
	CComPtr<ID2D1SolidColorBrush> pBrush = NULL;
	CComPtr<ID2D1SolidColorBrush> pHighlightingBrush = NULL;
	CComPtr<IDWriteTextFormat> pTextFormat = NULL;
	CComPtr<IDWriteTextLayout> pTextLayout = NULL;
	std::weak_ptr<FileManager> wpFileManager;
	
	D2D1_RECT_F windowPosition;
	D2D1_RECT_F layoutRect;
	static const unsigned int MAX_HIT_METRIC_SIZE = 50000;
	unsigned int hitMetricsCount = 0;
	std::unique_ptr<std::array<DWRITE_HIT_TEST_METRICS, MAX_HIT_METRIC_SIZE> > pMetricsBuffer;

public:
	LinkedListText(std::weak_ptr<FileManager> wpFileManager, const D2D1_RECT_F& windowPosition, float fontSize, D2D1::ColorF textColor = D2D1::ColorF(1.0, 1.0, 1.0), D2D1::ColorF selectionColor = D2D1::ColorF(0.2, 0.3, 0.5)) :
		wpFileManager(wpFileManager),
		windowPosition(windowPosition),
		fontSize(fontSize),
		layoutRect(D2D1_RECT_F {0, 0, 0, 0}),
		textColor(textColor),
		selectionColor(selectionColor)
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
	MouseCursorPos GetTextPos(float x, float y);
	std::wstring CreateSubstring(int startPos, int endPos);
	void SetHightlightText(int pos, int length);
	void ResetHighlightingMetricBuffer();
};





