/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */

// TranslucentDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TranslucentDialog.h"
#include "Utility.h"
#include "Application.h"
#include <algorithm>

// CTranslucentDialog �Ի���
IMPLEMENT_DYNAMIC(CTranslucentDialog,CDialog)
CTranslucentDialog::CTranslucentDialog(UINT nIDTemplate, LPCTSTR lpszFile, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
	, m_pStandardDlg(NULL)
	, m_pImage(Image::FromFile(lpszFile))
	, m_bEnableDrag(true)
	, m_bCenterAligned(true)
	, m_bBuffered(false)
{
	VERIFY( CUtility::IsFileExist(lpszFile) );
	VERIFY( m_pImage != NULL );
}

CTranslucentDialog::CTranslucentDialog(UINT nIDTemplate, UINT nImgID, LPCTSTR lpszType/* = _T("PNG")*/, HINSTANCE hResourceModule/* = NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
	, m_pStandardDlg(NULL)
	, m_pImage(CUtility::LoadImage(nImgID, lpszType, hResourceModule))
	, m_bEnableDrag(true)
	, m_bCenterAligned(true)
{
	VERIFY( m_pImage != NULL );
}

CTranslucentDialog::~CTranslucentDialog()
{
	if (m_pStandardDlg != NULL)
	{
		::delete m_pStandardDlg;
		m_pStandardDlg = NULL;
	}

	DllExports::GdipFree(m_pImage);
	m_pImage = NULL;

	/*for (auto it = m_translucentWndList.begin(); it != m_translucentWndList.end(); ++it){
		UnregisterTranslucentWnd((*it));
	}*/
}

BEGIN_MESSAGE_MAP(CTranslucentDialog, CDialog)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
//	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CTranslucentDialog::SetEnableDrag(bool bEnableDrag)
{
	m_bEnableDrag = bEnableDrag;
}

void CTranslucentDialog::SetCenterAligned(bool bCenterAligned)
{
	m_bCenterAligned = bCenterAligned;
}

void CTranslucentDialog::RegisterTranslucentWnd(CTranslucentWnd* translucentWnd)
{
	translucentWnd->SetRenderListener(this);
	m_translucentWndList.push_back(translucentWnd);
}

void CTranslucentDialog::UnregisterTranslucentWnd(CTranslucentWnd* translucentWnd)
{
	translucentWnd->SetRenderListener(NULL);

	TranslucentWndList::iterator it = std::find(m_translucentWndList.begin(), m_translucentWndList.end(), translucentWnd);
	if (it != m_translucentWndList.end())
		m_translucentWndList.erase(it);
}

BOOL CTranslucentDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	//RECT rc;
	//GetWindowRect(&rc);

	// must be WS_POPUP
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	VERIFY((dwStyle & WS_POPUP) != 0);
	VERIFY((dwStyle & WS_CHILD) == 0);

	m_blend.BlendOp = 0;
	m_blend.BlendFlags = 0;
	m_blend.AlphaFormat = 1;
	m_blend.SourceConstantAlpha = 255;//AC_SRC_ALPHA
	
	OnInitChildrenWnds();

	UpdateView();

	return TRUE;
}

BOOL CTranslucentDialog::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTranslucentDialog::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	//int width = m_rcWindow.right - m_rcWindow.left;
	//int height = m_rcWindow.bottom - m_rcWindow.top;

	//m_rcWindow.left = x;
	//m_rcWindow.top = y;
	//m_rcWindow.right = m_rcWindow.left + width;
	//m_rcWindow.bottom = m_rcWindow.top + height;

	if (m_pStandardDlg != NULL)
	{
		::SetWindowPos(m_pStandardDlg->GetSafeHwnd(), NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
}

void CTranslucentDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//m_rcWindow.right = m_rcWindow.left + cx;
	//m_rcWindow.bottom = m_rcWindow.top + cy;

	if (m_pStandardDlg != NULL)
	{
		::SetWindowPos(m_pStandardDlg->GetSafeHwnd(), NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
	}
}

int CTranslucentDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// Must be overlapped
	VERIFY((lpCreateStruct->style & WS_POPUP) == 0);
	VERIFY((lpCreateStruct->style & WS_CHILD) == 0);

	long nX = lpCreateStruct->x;
	long nY = lpCreateStruct->y;
	long nWidth = lpCreateStruct->cx;
	long nHeight = lpCreateStruct->cy;
	if (m_bCenterAligned)
	{
		RECT rcWindow = { 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight()};
		::AdjustWindowRect(&rcWindow, (DWORD)GetWindowLong(m_hWnd, GWL_STYLE), FALSE);
		nWidth = rcWindow.right - rcWindow.left;
		nHeight = rcWindow.bottom - rcWindow.top;
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWindow, 0);
		nX = rcWindow.left + (rcWindow.right - rcWindow.left - nWidth) / 2;
		nY = rcWindow.top + (rcWindow.bottom - rcWindow.top - nHeight) / 2;
	}

	nWidth = m_pImage->GetWidth();
	nHeight = m_pImage->GetHeight();

	//m_rcWindow.left = nX;
	//m_rcWindow.top = nY;
	//m_rcWindow.right = m_rcWindow.left + nWidth;
	//m_rcWindow.bottom = m_rcWindow.top + nHeight;

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pStandardDlg = CreateStandardDialog();
	if (m_pStandardDlg != NULL)
		m_pStandardDlg->ShowDialog();

	ModifyStyle(WS_CAPTION, WS_POPUP, 0);
	MoveWindow(nX, nY, nWidth, nHeight, TRUE);

	return 0;
}

typedef BOOL(WINAPI *MYFUNC)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);
static HMODULE hFuncInst = LoadLibrary(_T("User32.DLL"));
static MYFUNC UpdateLayeredWindowEx = (MYFUNC)::GetProcAddress(hFuncInst, "UpdateLayeredWindow");

void CTranslucentDialog::UpdateView()
{
	SIZE sizeWindow = { m_pImage->GetWidth(), m_pImage->GetHeight()};

	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if ((dwExStyle & 0x80000) != 0x80000)
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle | 0x80000);

	HDC hDC = ::GetDC(m_hWnd);
	HDC hdcMemory = CreateCompatibleDC(hDC);

	/* Should free*/
	HBITMAP hBitMap = CreateCompatibleBitmap(hDC, sizeWindow.cx, sizeWindow.cy);
	::SelectObject(hdcMemory, hBitMap);

	RECT rcWindow;
	GetWindowRect(&rcWindow);

	BITMAPINFOHEADER stBmpInfoHeader = { 0 };   
	int nBytesPerLine = ((sizeWindow.cx * 32 + 31) & (~31)) >> 3;
	stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);   
	stBmpInfoHeader.biWidth = sizeWindow.cx;   
	stBmpInfoHeader.biHeight = sizeWindow.cy;   
	stBmpInfoHeader.biPlanes = 1;   
	stBmpInfoHeader.biBitCount = 32;   
	stBmpInfoHeader.biCompression = BI_RGB;   
	stBmpInfoHeader.biClrUsed = 0;   
	stBmpInfoHeader.biSizeImage = nBytesPerLine * sizeWindow.cy;   

	PVOID pvBits = NULL;   
	HBITMAP hbmpMem = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, &pvBits, NULL, 0);
	//ASSERT(hbmpMem != NULL);
	
	if(hbmpMem)   
	{   
		memset(pvBits, 0, sizeWindow.cx * 4 * sizeWindow.cy);
		HGDIOBJ hbmpOld = ::SelectObject( hdcMemory, hbmpMem);

		//POINT ptWinPos = { m_rcWindow.left, m_rcWindow.top };
		POINT ptWinPos = { rcWindow.left, rcWindow.top };
		Gdiplus::Graphics graph(hdcMemory);
		graph.SetSmoothingMode(Gdiplus::SmoothingModeNone);

		graph.DrawImage(m_pImage, 0, 0, sizeWindow.cx, sizeWindow.cy);

		TranslucentWndList::const_iterator it;
		for (it = m_translucentWndList.begin(); it != m_translucentWndList.end(); ++it)
		{
			CTranslucentWnd* translucentWnd = *it;
			if (translucentWnd->IsVisible())
				translucentWnd->Render(graph);
		}

		POINT ptSrc = { 0, 0};
		UpdateLayeredWindowEx(m_hWnd, hDC, &ptWinPos, &sizeWindow, hdcMemory, &ptSrc, 0, &m_blend, 2);

		graph.ReleaseHDC(hdcMemory);
		::SelectObject(hdcMemory, hbmpOld);  
		//::CloseHandle(hbmpOld);
		::DeleteObject(hbmpMem); 
		//::CloseHandle(hbmpMem);
		FreeLibrary(hFuncInst);
	}
	::DeleteObject(hBitMap);
	::DeleteDC(hdcMemory);
	::ReleaseDC(m_hWnd, hDC);
}

void CTranslucentDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bEnableDrag)
	{
		::SendMessage( GetSafeHwnd(), WM_SYSCOMMAND, 0xF012, 0);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CTranslucentDialog::OnRenderEvent(CTranslucentWnd* translucentWnd)
{
	//if (!m_bBuffered)
		UpdateView();
}

void CTranslucentDialog::StartBuffered()
{
	if (m_bBuffered)
		EndBuffered();

	m_bBuffered = true;
}

void CTranslucentDialog::EndBuffered()
{
	if (m_bBuffered)
	{
		UpdateView();
		m_bBuffered = false;
	}
}

