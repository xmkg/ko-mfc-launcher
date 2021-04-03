/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


// TranlucentButton.cpp : implementation file
//

#include "stdafx.h"
#include "TranslucentButton.h"
#include "Utility.h"
#include ".\translucentbutton.h"


// CTranslucentButton

IMPLEMENT_DYNAMIC(CTranslucentButton, CTranslucentWnd)

CTranslucentButton::CTranslucentButton()
	: CTranslucentWnd()
	, m_pCurrImage(NULL)
	, m_bMouseOver(false)
	, m_bPressed(false)
	, m_bTracking(false)
	, m_bSelected(false)
	, m_pFont(::new Gdiplus::Font(_T("Arial"), 12))
	, m_format()
	, m_brush(Gdiplus::Color(0, 0, 0))
{
	m_imageList.resize(TWS_BUTTON_NUM, NULL);
	m_format.SetAlignment(Gdiplus::StringAlignmentCenter);
	m_format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
}

CTranslucentButton::~CTranslucentButton()
{
	::delete m_pFont;
	m_pFont = NULL;
	//DestroyImageList();
}


BEGIN_MESSAGE_MAP(CTranslucentButton, CTranslucentWnd)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//ON_MESSAGE(BM_SETCHECK, OnCheckChanged)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CTranslucentButton message handlers


void CTranslucentButton::PreSubclassWindow()
{
	//ModifyStyle(0, BS_OWNERDRAW);
	//VERIFY();
	CTranslucentWnd::PreSubclassWindow();
}

bool CTranslucentButton::LoadImageList(LPCTSTR szImageList[TWS_BUTTON_NUM])
{
	DestroyImageList();

	if (szImageList[TWS_NORMAL] == NULL)
		return false;

	m_imageList[TWS_NORMAL] = Image::FromFile(szImageList[TWS_NORMAL]);
	if (m_imageList[TWS_NORMAL] == NULL)
		return false;

	if (szImageList[TWS_MOUSEOVER] != NULL)
		m_imageList[TWS_MOUSEOVER] = Image::FromFile(szImageList[TWS_MOUSEOVER]);

	if (szImageList[TWS_PRESSED] != NULL)
		m_imageList[TWS_PRESSED] = Image::FromFile(szImageList[TWS_PRESSED]);

	if (szImageList[TWS_DISABLED] != NULL)
		m_imageList[TWS_DISABLED] = Image::FromFile(szImageList[TWS_DISABLED]);

	//if (szImageList[TWS_FOCUSED] != NULL)
	//	m_imageList[TWS_FOCUSED] = Image::FromFile(szImageList[TWS_FOCUSED]);

	if (szImageList[TWS_SELECTED] != NULL)
		m_imageList[TWS_SELECTED] = Image::FromFile(szImageList[TWS_SELECTED]);
	
	m_pCurrImage = m_imageList[TWS_NORMAL];

	VERIFY(SetWindowPos(NULL, -1, -1, m_pCurrImage->GetWidth(), m_pCurrImage->GetHeight(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE));
	return true;
}

bool CTranslucentButton::LoadImageList(UINT imageIDList[TWS_BUTTON_NUM], LPCTSTR lpszResType, HINSTANCE hInstance/* = NULL*/)
{
	DestroyImageList();

	if (imageIDList[TWS_NORMAL] == 0)
		return false;

	m_imageList[TWS_NORMAL] = CUtility::LoadImage(imageIDList[TWS_NORMAL], lpszResType, hInstance);
	if (m_imageList[TWS_NORMAL] == NULL)
		return false;

	if (imageIDList[TWS_MOUSEOVER] != 0)
		m_imageList[TWS_MOUSEOVER] = CUtility::LoadImage(imageIDList[TWS_MOUSEOVER], lpszResType, hInstance);

	if (imageIDList[TWS_PRESSED] != 0)
		m_imageList[TWS_PRESSED] = CUtility::LoadImage(imageIDList[TWS_PRESSED], lpszResType, hInstance);

	if (imageIDList[TWS_DISABLED] != 0)
		m_imageList[TWS_DISABLED] = CUtility::LoadImage(imageIDList[TWS_DISABLED], lpszResType, hInstance);

	//if (imageIDList[TWS_FOCUSED] != 0)
	//	m_imageList[TWS_FOCUSED] = CUtility::LoadImage(imageIDList[TWS_FOCUSED], lpszResType, hInstance);

	if (imageIDList[TWS_SELECTED] != 0)
		m_imageList[TWS_SELECTED] = CUtility::LoadImage(imageIDList[TWS_SELECTED], lpszResType, hInstance);

	m_pCurrImage = m_imageList[TWS_NORMAL];

	VERIFY(SetWindowPos(NULL, -1, -1, m_pCurrImage->GetWidth(), m_pCurrImage->GetHeight(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE));
	return true;
}

void CTranslucentButton::UpdateState()
{
	Gdiplus::Image* prevImage = m_pCurrImage;

	if (m_bMouseOver)
	{
		if (m_bPressed)
		{
			if (m_imageList[TWS_PRESSED] != NULL)
				m_pCurrImage = m_imageList[TWS_PRESSED];
			else
				m_pCurrImage = m_imageList[TWS_NORMAL];
		}
		else
		{
			if (m_imageList[TWS_MOUSEOVER] != NULL)
				m_pCurrImage = m_imageList[TWS_MOUSEOVER];
			else
				m_pCurrImage = m_imageList[TWS_NORMAL];
		}
	}
	else if (m_bDisabled)
	{
		if (m_imageList[TWS_DISABLED] != NULL)
			m_pCurrImage = m_imageList[TWS_DISABLED];
		else
			m_pCurrImage = m_imageList[TWS_NORMAL];
	}
	else if (m_bSelected)
	{
		if (m_imageList[TWS_SELECTED] != NULL)
			m_pCurrImage = m_imageList[TWS_SELECTED];
		else
			m_pCurrImage = m_imageList[TWS_NORMAL];
	}
	else
	{
		m_pCurrImage = m_imageList[TWS_NORMAL];
	}

	if (prevImage != m_pCurrImage)
	{
		NotifyRender();
	}
}

void CTranslucentButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = (_TrackMouseEvent(&tme) == FALSE) ? false : true;
	}

	CTranslucentWnd::OnMouseMove(nFlags, point);
}

LRESULT CTranslucentButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bMouseOver = false;
	m_bTracking = false;
	UpdateState();
	return 0;
}

LRESULT CTranslucentButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	m_bMouseOver = true;
	UpdateState();
	return 0;
}

void CTranslucentButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bPressed = true;
	UpdateState();
	CTranslucentWnd::OnLButtonDown(nFlags, point);
}

void CTranslucentButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bPressed = false;
	RECT rc;
	GetClientRect(&rc);
	BOOL bIn = ::PtInRect(&rc, point);
	if (bIn)
	{
		UINT uStyle = (UINT)::GetWindowLong(m_hWnd, GWL_STYLE) & 0xff;
		if ((uStyle == BS_RADIOBUTTON) || (uStyle == BS_CHECKBOX) || (uStyle == BS_AUTOCHECKBOX) || (uStyle == BS_AUTORADIOBUTTON))
			m_bSelected = !m_bSelected;
		else
			m_bSelected = false;
	}
	else
	{
		m_bMouseOver = false;
	}

	UpdateState();
	CTranslucentWnd::OnLButtonUp(nFlags, point);
}

//LRESULT CTranslucentButton::OnCheckChanged(WPARAM wParam, LPARAM lParam)
//{
//	if (wParam == BST_CHECKED)
//		m_bSelected = !m_bSelected;
//	else
//		m_bSelected = false;
//	UpdateState();
//	return 0;
//}

void CTranslucentButton::SetChecked(bool b)
{
	m_bSelected = b;
	UpdateState();
}

bool CTranslucentButton::IsChecked() const
{ 
	return m_bSelected;
}


//void CTranslucentButton::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
//{
//	
//}

void CTranslucentButton::SetFont(const WCHAR* familyName, float emSize, int style/* = Gdiplus::FontStyleRegular*/, Gdiplus::Unit unit/* = Gdiplus::UnitPixel*/)
{
	::delete m_pFont;
	m_pFont = ::new Gdiplus::Font(familyName, emSize, style, unit);
}

void CTranslucentButton::SetFormat(Gdiplus::StringAlignment align/* = Gdiplus::StringAlignmentCenter*/, Gdiplus::StringAlignment lineAlign/* = Gdiplus::StringAlignmentCenter*/)
{
	m_format.SetAlignment(align);
	m_format.SetLineAlignment(lineAlign);
}

void CTranslucentButton::SetColor(const Gdiplus::Color& color)
{
	m_brush.SetColor(color);
}

void CTranslucentButton::Render(Gdiplus::Graphics& g)
{
	if (m_pCurrImage != NULL)
	{
		CRect rc;
		GetWindowRect(&rc);
		GetParent()->ScreenToClient(&rc);

		Gdiplus::RectF rect((float)rc.left, (float)rc.top, (float)rc.Width(), (float)rc.Height());
		g.DrawImage(m_pCurrImage, rect);

		CString szText;
		GetWindowText(szText);
		if (szText.GetLength() > 0)
		{
			g.DrawString(szText, szText.GetLength(), m_pFont, rect, &m_format, &m_brush);
		}
	}
}
