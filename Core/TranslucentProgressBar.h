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

// CTranslucentProgressBar

class CTranslucentProgressBar : public CTranslucentWnd
{
	DECLARE_DYNAMIC(CTranslucentProgressBar)

public:
	CTranslucentProgressBar();
	virtual ~CTranslucentProgressBar();

	// Set the background image
	bool SetBgImage(LPCTSTR szBgImage);
	bool SetBgImage(UINT nBgImageID, LPCTSTR lpszResType = _T("PNG"), HINSTANCE hInstance = NULL);

	// Set the foreground image
	bool SetFgImage(LPCTSTR szFgImage);
	bool SetFgImage(UINT nFgImageID, LPCTSTR lpszResType = _T("PNG"), HINSTANCE hInstance = NULL);
	
	void GetRange(int& lower, int& upper) const;
	int GetPos() const;
	int GetStep() const;
	void StepIt();
	BOOL SetStep(int nStep);
	void OffsetPos(int nPos);
	BOOL SetPos(int nPos);
	void SetRange(int nLower, int nUpper);

	virtual void Render(Gdiplus::Graphics& g);

	void SetFont(const WCHAR* familyName, float emSize, int style = Gdiplus::FontStyleRegular, Gdiplus::Unit unit = Gdiplus::UnitPixel);
	void SetFormat(Gdiplus::StringAlignment align = Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignment lineAlign = Gdiplus::StringAlignmentCenter);
	void SetColor(const Gdiplus::Color& color);

	Gdiplus::Font* GetFont() { return m_pFont; }
	Gdiplus::StringFormat& GetFormat() { return m_format; }
	Gdiplus::SolidBrush& GetBrush() { return m_brush; }

protected:
	DECLARE_MESSAGE_MAP()

private:
	int m_nLower;
	int m_nUpper;
	int m_nStep;
	int m_nPos;

	Gdiplus::StringFormat m_format;
	Gdiplus::Font* m_pFont;
	Gdiplus::SolidBrush m_brush;
};


