/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


#pragma once

#include "TranslucentWnd.h"

class CTranslucentLabel:public CTranslucentWnd
{
	DECLARE_DYNAMIC(CTranslucentLabel)
public:
	CTranslucentLabel();
	~CTranslucentLabel();

	virtual void Render(Gdiplus::Graphics& g);

	void SetFont(const WCHAR* familyName, float emSize, int style = Gdiplus::FontStyleRegular, Gdiplus::Unit unit = Gdiplus::UnitPixel);
	void SetFormat(Gdiplus::StringAlignment align = Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignment lineAlign = Gdiplus::StringAlignmentCenter);
	void SetColor(const Gdiplus::Color& color);

	Gdiplus::Font* GetFont() { return m_pFont; }
	Gdiplus::StringFormat& GetFormat() { return m_format; }
	Gdiplus::SolidBrush& GetBrush() { return m_brush; }
private:
	DECLARE_MESSAGE_MAP()
	//virtual void PreSubclassWindow();
	//virtual void UpdateState();
	Gdiplus::StringFormat m_format;
	Gdiplus::Font* m_pFont;
	Gdiplus::SolidBrush m_brush;
};

