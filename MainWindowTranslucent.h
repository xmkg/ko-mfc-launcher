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

#include "TranslucentDialog.h"
#include "TranslucentButton.h"
#include "TranslucentProgressBar.h"
#include "TranslucentLabel.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ftp\FTPClient.h"
#include "ftp\FTPDataTypes.h"
#include "threadpool.h"
#include "compress/7zpp.h"

// CMainWindowTranslucent dialog

class CMainWindowTranslucent : public CTranslucentDialog, public nsFTP::CFTPClient::CNotification
{
	DECLARE_DYNAMIC(CMainWindowTranslucent)

public:
	CMainWindowTranslucent(LPCTSTR lpszFile, CWnd* pParent = NULL);   // standard constructor
	CMainWindowTranslucent(UINT nImgID, LPCTSTR lpszType = _T("PNG"), HINSTANCE hResourceModule = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainWindowTranslucent();

// Dialog Data
	enum { IDD = IDD_TRANSLUCENTDIALOG };

	virtual CStandardDialog* CreateStandardDialog();
	virtual void OnInitChildrenWnds();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT & cs);

	SevenZip::SevenZipLibrary lib;
	boost::shared_ptr<CUpdaterSession> m_pUpdaterSession;
	nsFTP::CFTPClient * m_pFTPClient;
	ThreadPool * WorkerThreadPool;
	DECLARE_MESSAGE_MAP()

public:
	CTranslucentButton m_btnOptions;
	CTranslucentProgressBar m_progressBar;
	CTranslucentButton m_btnHomepage;
	CTranslucentButton m_btnStart;
	CTranslucentButton m_btnClose;
	CTranslucentLabel m_lblStatus;


	/* Launcher session */
	void onConnectionFailure(void);
	void onResolveFailure(void);
	void onConnected(void);
	void onDisconnected(void);
	void onPacketReceived(Packet* pkt);
	void HandleVersionReq(Packet * pkt);
	void HandleDownloadInfo(Packet * pkt);

private:
	void LaunchExecutable(const std::wstring & strExecutablePath, bool bCloseThis = false) const noexcept;
    bool CheckUpdateLauncher();
	void DownloadNextPatch();
	void AbortUpdateAndExit();

	/* FTP Handlers*/
	virtual void OnInternalError(const tstring& /*strErrorMsg*/, const tstring& /*strFileName*/, DWORD /*dwLineNr*/);

	virtual void OnBeginReceivingData();
	virtual void OnEndReceivingData(long /*lReceivedBytes*/);
	virtual void OnBytesReceived(const nsFTP::TByteVector& /*vBuffer*/, long /*lReceivedBytes*/);
	virtual void OnReceiveFileResume(long lSeekOffset);



	virtual void OnPreReceiveFile(const tstring& /*strSourceFile*/, const tstring& /*strTargetFile*/, long /*lFileSize*/);
	virtual void OnPostReceiveFile(const tstring& /*strSourceFile*/, const tstring& /*strTargetFile*/, long /*lFileSize*/);
	

	virtual void OnSendCommand(const nsFTP::CCommand& /*Command*/, const nsFTP::CArg& /*Arguments*/);
	virtual void OnResponse(const nsFTP::CReply& /*Reply*/);

	/* NOT GOING TO IMPLEMENT THESE*/
	virtual void OnBytesSent(const nsFTP::TByteVector& /*vBuffer*/, long /*lSentBytes*/) {}
	virtual void OnPostSendFile(const tstring& /*strSourceFile*/, const tstring& /*strTargetFile*/, long /*lFileSize*/){}
	virtual void OnPreSendFile(const tstring& /*strSourceFile*/, const tstring& /*strTargetFile*/, long /*lFileSize*/){}

	
public:
	afx_msg void OnBnClickedButton3() noexcept;
	afx_msg void OnBnClickedButton4() noexcept;
	afx_msg void OnBnClickedButton2()  noexcept;
	afx_msg void OnBnClickedButton1() noexcept;
};
