
/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */

// ContentDialog.cpp : implementation file
//

#include "stdafx.h"
#include "StandardDialog.h"


// CStandardDialog dialog

IMPLEMENT_DYNAMIC(CStandardDialog, CDialog)
CStandardDialog::CStandardDialog(UINT nIDTemplate, COLORREF clrTransparent/* = RGB(0, 255, 0)*/, CWnd* pParentWnd/* = NULL*/)
	: CDialog(nIDTemplate, pParentWnd)
	, m_clrTransparent(clrTransparent)
{
}

CStandardDialog::~CStandardDialog()
{
}

BEGIN_MESSAGE_MAP(CStandardDialog, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CStandardDialog message handlers

void CStandardDialog::OnOK()
{
	CWnd* hParent = GetParent();
	if (hParent != NULL)
	{
		HWND hWnd = hParent->m_hWnd; 
		::SendMessage(hWnd, WM_CLOSE, 0, 0); 
	}

	CDialog::OnOK();
}

void CStandardDialog::OnCancel()
{
	CWnd* hParent = GetParent();
	if (hParent != NULL)
	{
		HWND hWnd = hParent->m_hWnd; 
		::SendMessage(hWnd, WM_CLOSE, 0, 0); 
	}

	CDialog::OnCancel();
}

void CStandardDialog::EndDialog(int nResult)
{
	CWnd* hParent = GetParent();
	if (hParent != NULL)
	{
		HWND hWnd = hParent->m_hWnd; 
		::SendMessage(hWnd, WM_CLOSE, 0, 0); 
	}

	CDialog::EndDialog(nResult);
}

HBRUSH CStandardDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG)
		return m_brush;

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CStandardDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	VERIFY((dwStyle & WS_POPUP) != 0);
	VERIFY((dwStyle & WS_BORDER) == 0);
	VERIFY((dwStyle & WS_SIZEBOX) == 0);
	VERIFY((dwStyle & WS_DLGFRAME) == 0);

	m_brush.CreateSolidBrush(m_clrTransparent);
	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	VERIFY((dwExStyle & WS_EX_APPWINDOW) == 0);
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle | 0x80000);

	HMODULE hInst = LoadLibrary(_T("User32.DLL")); 
	typedef BOOL (WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD); 
	MYFUNC SetLayeredWindowAttributes = NULL; 
	SetLayeredWindowAttributes = (MYFUNC)::GetProcAddress(hInst, "SetLayeredWindowAttributes"); 
	SetLayeredWindowAttributes(GetSafeHwnd(), m_clrTransparent, 0, 1); 
	::FreeLibrary(hInst);

	return TRUE;
}

void CStandardDialog::ShowDialog()
{
	Create(m_lpszTemplateName, m_pParentWnd);
	ShowWindow(SW_SHOW);
}
