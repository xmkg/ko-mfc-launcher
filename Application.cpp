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
#include "MainWindowTranslucent.h"
#include "LogWriter.h"


// CTranslucentApp

BEGIN_MESSAGE_MAP(CTranslucentApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTranslucentApp ����

CTranslucentApp::CTranslucentApp()
{
	
}



CTranslucentApp theApp;
ULONG_PTR gdiplusToken;

BOOL CTranslucentApp::InitInstance()
{
	CWinApp::InitInstance();

	AfxEnableControlContainer();

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CreateDirectory(L"logs", NULL);
	CLogWriter::prepare();

	CLogWriter::Log->info("Updater started");
	
	CMainWindowTranslucent dlg(IDB_BACKGROUND);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}

int CTranslucentApp::ExitInstance()
{
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return CWinApp::ExitInstance();
}

