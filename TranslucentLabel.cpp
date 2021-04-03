/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


#include "stdafx.h"
#include "TranslucentLabel.h"

IMPLEMENT_DYNAMIC(CTranslucentLabel, CTranslucentWnd)

CTranslucentLabel::CTranslucentLabel():CTranslucentWnd()
, m_pFont(::new Gdiplus::Font(_T("Arial"), 12))
, m_format()
, m_brush(Gdiplus::Color(0, 0, 0))
{
	m_format.SetAlignment(Gdiplus::StringAlignmentCenter);
	m_format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
}


CTranslucentLabel::~CTranslucentLabel()
{
	::delete m_pFont;
	m_pFont = NULL;
}

BEGIN_MESSAGE_MAP(CTranslucentLabel, CTranslucentWnd)
END_MESSAGE_MAP()

void CTranslucentLabel::Render(Gdiplus::Graphics & g)
{
	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);

	Gdiplus::RectF rect((float)rc.left, (float)rc.top, (float)rc.Width(), (float)rc.Height());
	CString szText;
	GetWindowText(szText);
	if (szText.GetLength() > 0)
	{
		g.DrawString(szText, szText.GetLength(), m_pFont, rect, &m_format, &m_brush);
	}
}

void CTranslucentLabel::SetFont(const WCHAR * familyName, float emSize, int style, Gdiplus::Unit unit)
{
	::delete m_pFont;
	m_pFont = ::new Gdiplus::Font(familyName, emSize, style, unit);
}

void CTranslucentLabel::SetFormat(Gdiplus::StringAlignment align, Gdiplus::StringAlignment lineAlign)
{
	m_format.SetAlignment(align);
	m_format.SetLineAlignment(lineAlign);
}

void CTranslucentLabel::SetColor(const Gdiplus::Color & color)
{
	m_brush.SetColor(color);
}
