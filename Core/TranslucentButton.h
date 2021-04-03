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

// CTranslucentButton
class CTranslucentButton : public CTranslucentWnd
{
	DECLARE_DYNAMIC(CTranslucentButton)

public:
	CTranslucentButton();
	virtual ~CTranslucentButton();
	
	bool LoadImageList(LPCTSTR szImageList[TWS_BUTTON_NUM]);
	bool LoadImageList(UINT imageIDList[TWS_BUTTON_NUM], LPCTSTR lpszResType = _T("PNG"), HINSTANCE hInstance = NULL);

	virtual void Render(Gdiplus::Graphics& g);

	// for checkbox or radiobutton
	void SetChecked(bool b);
	bool IsChecked() const;

	void SetFont(const WCHAR* familyName, float emSize, int style = Gdiplus::FontStyleRegular, Gdiplus::Unit unit = Gdiplus::UnitPixel);
	void SetFormat(Gdiplus::StringAlignment align = Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignment lineAlign = Gdiplus::StringAlignmentCenter);
	void SetColor(const Gdiplus::Color& color);
	
	Gdiplus::Font* GetFont() { return m_pFont; }
	Gdiplus::StringFormat& GetFormat() { return m_format; }
	Gdiplus::SolidBrush& GetBrush() { return m_brush; }

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual void UpdateState();

	Gdiplus::Image* m_pCurrImage;
	bool m_bMouseOver;
	bool m_bPressed;
	bool m_bSelected;
	bool m_bTracking;

	Gdiplus::StringFormat m_format;
	Gdiplus::Font* m_pFont;
	Gdiplus::SolidBrush m_brush;

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnCheckChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


