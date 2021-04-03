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
#include "Application.h"
#include "MainWindowStandard.h"



// CMainWindowStandard dialog

IMPLEMENT_DYNAMIC(CMainWindowStandard, CStandardDialog)
CMainWindowStandard::CMainWindowStandard(CWnd* pParent/* = NULL*/)
	: CStandardDialog(CMainWindowStandard::IDD, RGB(0, 255, 0), pParent)
{
}

CMainWindowStandard::~CMainWindowStandard()
{
}


void CMainWindowStandard::DoDataExchange(CDataExchange* pDX)
{
	CStandardDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER2, m_webBrowser);
	m_webBrowser.MoveWindow(24, 232, 635,381);
	m_webBrowser.Navigate(L"https://www.youtube.com/embed/Bn8Wqzj5PRc", NULL, NULL, NULL, NULL);
	
}


BEGIN_MESSAGE_MAP(CMainWindowStandard, CStandardDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CMainWindowStandard message handlers

void CMainWindowStandard::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	::MessageBox(NULL, _T("Hello"), _T("Hello World"), MB_OK);
}

void CMainWindowStandard::OnOK()
{
	CStandardDialog::OnOK();
}

BOOL CMainWindowStandard::OnInitDialog()
{
	
	CStandardDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMainWindowStandard::PreCreateWindow(CREATESTRUCT & cs)
{
	cs.cy = 683; // width
	cs.cx = 720; // height
	
	return CStandardDialog::PreCreateWindow(cs);
}
