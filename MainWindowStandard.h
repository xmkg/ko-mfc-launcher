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

#include "StandardDialog.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "Core\TranslucentButton.h"
#include "Core\Explorer.h"

// CMainWindowStandard dialog

class CMainWindowStandard : public CStandardDialog
{
	DECLARE_DYNAMIC(CMainWindowStandard)

public:
	CMainWindowStandard(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainWindowStandard();

// Dialog Data
	enum { IDD = IDD_STANDARDDIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	virtual BOOL PreCreateWindow(CREATESTRUCT & cs);
	CExplorer m_webBrowser;
};
