/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


// DemoTranslucentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "MainWindowTranslucent.h"
#include "MainWindowStandard.h"
#include "OptionsDialog.h"
#include "LogWriter.h"

#include <queue>
#include <codecvt>

std::queue<std::string> patchQueue;
const char progressbar_string[] = "Downloading %s [%d kB of %d kB] @ %d KB/s (%\% %.2f)";

static float CalculatePercent(const float current, const float maximum) noexcept
{
	if (current == 0)
		return 0;
	return static_cast<float>(static_cast<float>((current / maximum)) * 100);
}

static std::string WideStringToStdString(const tstring & source) noexcept
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	static std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(source);
}

static std::wstring StdStringToWideString(const std::string & source) noexcept
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	static std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.from_bytes(source);
}

struct UpdateInformation
{
public:
	long CurrentFileSize;
	long CurrentFileDownloaded;
	long DownloadSpeed;
	long LastDownloadSpeedUpdate;
	long LastDownloadSpeedFileSize;
	std::string Filename;
	long TotalCountOfUpdates;
	long DownloadedUpdates;

	void CalculateDownloadSpeed() noexcept
	{
		long currentTickCount = GetTickCount();
		if (currentTickCount - LastDownloadSpeedUpdate >= 1000) {
			
			DownloadSpeed = ((CurrentFileDownloaded - LastDownloadSpeedFileSize)) / 1024;
			LastDownloadSpeedUpdate = currentTickCount;
			LastDownloadSpeedFileSize = CurrentFileDownloaded;
		}
	}

	void UpdateFileInfo(const std::string & newFileName, const long size) noexcept
	{
		Filename = newFileName;
		CurrentFileSize = size;
		CurrentFileDownloaded = 0;
		DownloadSpeed = 0;
		LastDownloadSpeedUpdate = GetTickCount();
		LastDownloadSpeedFileSize = 0;
	}

} m_UpdateInformation;


// CMainWindowTranslucent dialog

IMPLEMENT_DYNAMIC(CMainWindowTranslucent, CTranslucentDialog)
CMainWindowTranslucent::CMainWindowTranslucent(LPCTSTR lpszFile, CWnd* pParent /*=NULL*/)
	: CTranslucentDialog(CMainWindowTranslucent::IDD, lpszFile, pParent),WorkerThreadPool(nullptr),m_pFTPClient(nullptr)
{
}

CMainWindowTranslucent::CMainWindowTranslucent(UINT nImgID, LPCTSTR lpszType/* = _T("PNG")*/, HINSTANCE hResourceModule/* = NULL*/, CWnd* pParent /*=NULL*/)
	: CTranslucentDialog(CMainWindowTranslucent::IDD, nImgID, lpszType, hResourceModule, pParent), WorkerThreadPool(nullptr), m_pFTPClient(nullptr)
{
	try
	{
			char * strTempFN = "decomp.ress";
			#ifdef _WIN64
			CLogWriter::Log->info("Loading 64-bit version of the decompression library");
			FILE * pFile = fopen("lzma_x64.dll", "rb");
			
			#else
			CLogWriter::Log->info("Loading 32-bit version of the decompression library");
			FILE * pFile = fopen("lzma_x86.dll", "rb");
			#endif

			char buf[BUFSIZ];
			size_t size;
			if (pFile != nullptr)
			{

				FILE * pTmpFile = fopen(strTempFN, "wb");  // Create the file
				while ((size = fread_s(buf, BUFSIZ, 1, 1, pFile)))
				{
					fwrite(buf, 1, size, pTmpFile);
				}
				fclose(pTmpFile);
				fclose(pFile);
			}
			lib.Load(StdStringToWideString(std::string(strTempFN)).c_str());
	}
	catch (...) {
		CLogWriter::Log->error("Failed whilst trying to load decompression library");
		TRACE("LZMA LIB LOAD FAIL!!");
		::MessageBoxA(m_hWnd, "LZMA library load failure!\nlzma_x86.dll is a crucial part of the update system, and it appears missing.\nPlease ensure lzma_x86.dll is present.", "LZMA Error", MB_ICONWARNING);
		exit(-1);
	}
	/* Delete the old launcher (if exist) */
	if (DeleteFile(L"Launcher.old") > 0) {
		CLogWriter::Log->info("Deleted old launcher");
	}
	
	/* Delete the knight log file */
	if (DeleteFile(L"log.klg") > 0) {
		CLogWriter::Log->info("Deleted game log");
	}
	
	if (GameOptions::Read()) {
		CLogWriter::Log->info("Game option file read");
	}
	
	CUpdaterSession::RunIOService();
	WorkerThreadPool = new ThreadPool(4);
	if (!CUpdaterSession::ReadConfiguration())
	{
		CLogWriter::Log->error("Cannot read configuration file!");
		::MessageBoxA(NULL, "Configuration file (\"launcher.conf\") is missing!", "Warning", MB_ICONWARNING);
		exit(EXIT_FAILURE);
	}


}

void CMainWindowTranslucent::AbortUpdateAndExit() {
	CLogWriter::Log->info("Aborting update");
	if (m_pUpdaterSession)
	{
		CLogWriter::Log->info("Aborting updater session");
		m_pUpdaterSession->Disconnect();
		m_pUpdaterSession.reset();
		m_pUpdaterSession = nullptr;
	}
	CUpdaterSession::StopIOService();
	if (m_pFTPClient)
	{
		CLogWriter::Log->info("Aborting FTP session");
		if (m_pFTPClient->IsConnected())
		{
			m_pFTPClient->Abort();
			m_pFTPClient->Logout();
		}
		delete m_pFTPClient;
		m_pFTPClient = nullptr;
	}

	//delete WorkerThreadPool;
	lib.Free();

	DeleteFile(L"decomp.ress");
	CLogWriter::Log->info("Terminating process");
	TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
}

CMainWindowTranslucent::~CMainWindowTranslucent()
{
	AbortUpdateAndExit();
}

#pragma region ///	UI

void CMainWindowTranslucent::DoDataExchange(CDataExchange* pDX)
{
	CTranslucentDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnOptions);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressBar);
	DDX_Control(pDX, IDC_BUTTON2, m_btnHomepage);
	DDX_Control(pDX, IDC_BUTTON3, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON4, m_btnClose);
	DDX_Control(pDX, IDC_STATIC2, m_lblStatus);
}

BOOL CMainWindowTranslucent::PreCreateWindow(CREATESTRUCT & cs)
{
	cs.cy = 683; // width
	cs.cx = 720; // height
	return CTranslucentDialog::PreCreateWindow(cs);
}





BEGIN_MESSAGE_MAP(CMainWindowTranslucent, CTranslucentDialog)
	ON_BN_CLICKED(IDC_BUTTON3, &CMainWindowTranslucent::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMainWindowTranslucent::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainWindowTranslucent::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMainWindowTranslucent::OnBnClickedButton1)
END_MESSAGE_MAP()


CStandardDialog* CMainWindowTranslucent::CreateStandardDialog()
{
	return ::new CMainWindowStandard(this);
}

void CMainWindowTranslucent::OnInitChildrenWnds()
{
	UINT imgList_Options[] = { IDB_BTNOPTIONS_DEFAULT,IDB_BTNOPTIONS_DISABLED,IDB_BTNOPTIONS_HOVERED,IDB_BTNOPTIONS_PRESSED,IDB_BTNOPTIONS_PRESSED };
	UINT imgList_Homepage[] = { IDB_BTNHOMEPAGE_DEFAULT,IDB_BTNHOMEPAGE_DISABLED,IDB_BTNHOMEPAGE_HOVERED,IDB_BTNHOMEPAGE_PRESSED,IDB_BTNHOMEPAGE_PRESSED };
	UINT imgList_Close[] = { IDB_BTNCLOSE_DEFAULT,IDB_BTNCLOSE_DISABLED,IDB_BTNCLOSE_HOVERED,IDB_BTNCLOSE_PRESSED,IDB_BTNCLOSE_PRESSED };
	UINT imgList_Start[] = { IDB_BTNSTART_DEFAULT,IDB_BTNSTART_DISABLED,IDB_BTNSTART_HOVERED,IDB_BTNSTART_PRESSED,IDB_BTNSTART_PRESSED };
	m_btnOptions.LoadImageList(imgList_Options);
	m_btnHomepage.LoadImageList(imgList_Homepage);
	m_btnStart.LoadImageList(imgList_Start);
	m_btnClose.LoadImageList(imgList_Close);
	m_btnStart.EnableWindow(false);
	m_progressBar.SetFgImage(IDB_PROGRESSBAR_FOREGROUND);
	m_progressBar.SetBgImage(IDB_PROGRESSBAR_BACKGROUND);
	m_progressBar.SetPos(0);
	m_progressBar.SetFont(L"Lucida Sans Unicode", 10.11f);
	m_progressBar.SetWindowTextW(L"");

	m_lblStatus.SetFont(L"Lucida Sans Unicode", 11.0f);
	m_lblStatus.SetWindowTextW(L"Checking for updates motherfucker..");
	m_lblStatus.SetFormat(Gdiplus::StringAlignment::StringAlignmentFar);
	m_lblStatus.SetColor(Gdiplus::Color::AntiqueWhite);
	m_progressBar.SetColor(Gdiplus::Color::AntiqueWhite);
	RegisterTranslucentWnd(&m_btnOptions);
	RegisterTranslucentWnd(&m_progressBar);
	RegisterTranslucentWnd(&m_btnHomepage);
	RegisterTranslucentWnd(&m_btnStart);
	RegisterTranslucentWnd(&m_btnClose);
	RegisterTranslucentWnd(&m_lblStatus);
	m_btnStart.MoveWindow(518, 648, 149, 59);
	m_btnOptions.MoveWindow(23, 649, 107, 29);
	m_btnHomepage.MoveWindow(139, 649, 107, 29);
	m_btnClose.MoveWindow(648, 175, 33, 33);
	m_progressBar.MoveWindow(25, 691, 471, 17);
	m_lblStatus.MoveWindow(248, 649, 250, 29);

	m_pUpdaterSession = CUpdaterSession::create();
	m_pUpdaterSession->SetLauncherDlg(this);
	m_pUpdaterSession->Connect();

}

#pragma endregion

#pragma region ///	Launcher session

void CMainWindowTranslucent::onPacketReceived(Packet * pkt)
{
	switch (pkt->GetOpcode())
	{
	case LS_VERSION_REQ:
		HandleVersionReq(pkt);
		break;
	case LS_DOWNLOADINFO_REQ:
		HandleDownloadInfo(pkt);
		break;
	}
}

void CMainWindowTranslucent::HandleVersionReq(Packet * pkt)
{
	uint16 sVersion = pkt->read<int16>();
	CLogWriter::Log->info("Version request packet received, server version is {}, client version is {}", sVersion, CUpdaterSession::m_usLastVersion);
	TRACE("Version req packet received, server version %d", sVersion);
	if (sVersion == CUpdaterSession::m_usLastVersion)
	{
		CLogWriter::Log->info("Client is currently up-to-date", sVersion);
		TRACE("Client is up-to-date");
		//Downloading patch1535.7z [30.727 kB of 44.775 kB] @ 223.1 kb/s  %p%
		m_progressBar.SetPos(100);
		m_lblStatus.SetWindowTextW(L"Update completed.");
		m_btnStart.EnableWindow(true);

		/* No longer needed */
		m_pUpdaterSession->Disconnect();
	}
	else if (sVersion > CUpdaterSession::m_usLastVersion)
	{
		CLogWriter::Log->info("Update required, requesting patch list from the update server");
		m_pUpdaterSession->SendDownloadListReq(CUpdaterSession::m_usLastVersion);
		m_lblStatus.SetWindowTextW(L"Update required, requestint patch list..");
	}
	else
	{
		CLogWriter::Log->info("Client version is invalid");
		m_lblStatus.SetWindowTextW(L"Invalid client version.");
		/* No longer needed */
		m_pUpdaterSession->Disconnect();
	}
}

void CMainWindowTranslucent::HandleDownloadInfo(Packet * pkt)
{
	std::string strFtpServer, strFtpPath;
	uint16 sPatchCount = 0;

	(*pkt) >> strFtpServer >> strFtpPath >> sPatchCount;
	CLogWriter::Log->info("Download info received");
	CLogWriter::Log->info("Host info (Protocol : FTP, Address : {}, Path : {})", strFtpServer.c_str(), strFtpPath.c_str());
	CLogWriter::Log->info("Required update count : {}", sPatchCount);
	TRACE("HandleDownloadInfo >> FTP host : %s, path : %s, patch count %d\n", strFtpServer.c_str(), strFtpPath.c_str(), sPatchCount);
	m_UpdateInformation.TotalCountOfUpdates = sPatchCount;
	m_UpdateInformation.DownloadedUpdates = 0;
	for (int i = 0; i < sPatchCount; i++) {
		std::string val;
		(*pkt) >> val;
		CLogWriter::Log->info("[{}] Queuing {} into update queue", i, val.c_str());
		TRACE("HandleDownloadInfo >> queuing %s into patch queue..", val.c_str());
		patchQueue.push(val);
	}

	/* No longer needed */
	m_pUpdaterSession->Disconnect();

	m_pFTPClient = new nsFTP::CFTPClient();
	m_pFTPClient->AttachObserver(this);

	std::wstring wstrFtpServer = StdStringToWideString(strFtpServer);
	std::wstring wstrFtpPath = StdStringToWideString(strFtpPath);
	CLogWriter::Log->info("Trying to connect to the FTP server");
	nsFTP::CLogonInfo logonInfo(wstrFtpServer);
	

	m_pFTPClient->Login(logonInfo);
	if (m_pFTPClient->IsConnected())
	{
		CLogWriter::Log->info("File transfer protocol connection established");
		m_lblStatus.SetWindowTextW(L"FTP connection successful.\nStarting update now..");
		if (m_pFTPClient->ChangeWorkingDirectory(wstrFtpPath) == 0x00 /*ftp_ok*/)
		{
			CLogWriter::Log->warn("Starting update batch");
			DownloadNextPatch();
		}
		else
		{
			CLogWriter::Log->warn("File transfer protocol connection is established but path is missing!");
			m_lblStatus.SetWindowTextW(L"Update failed.\nFTP server path is missing.");
		}
	}
	else
	{
		CLogWriter::Log->warn("File transfer protocol server is offline");
		m_lblStatus.SetWindowTextW(L"FTP server is unavailable.\nPlease try again later.");
	}
}

void CMainWindowTranslucent::LaunchExecutable(const std::wstring & strExecutablePath, bool bCloseThis) const noexcept
{

	CLogWriter::Log->info() << "Launching " << WideStringToStdString(strExecutablePath);
	STARTUPINFO processStartupInfo;
	PROCESS_INFORMATION processInformation;
	ZeroMemory(&processInformation, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&processStartupInfo, sizeof(STARTUPINFO));
	DWORD dwProcessCreationFlags =  CREATE_UNICODE_ENVIRONMENT;
	
	wchar_t buf[MAX_PATH];
	_wgetcwd(buf, MAX_PATH);

	
	StrCatW(buf, L"\\");
	StrCatW(buf, strExecutablePath.c_str());
	::MessageBoxW(m_hWnd, buf, L"path", 0);

	bool bSucceeded = CreateProcessW(
		/* Module name */
		buf,
		/* Command line */
		L"test test",
		/* Process handle not inheritable */
		NULL,
		/* Thread handle not inheritable */
		NULL, 
		/* Disable handle inheritance */
		false, 
		/* No creation flags */
		dwProcessCreationFlags, 
		/* Use parent's environment block */
		NULL, 
		/* Use parent's starting directory */
		NULL,
		&processStartupInfo, 
		&processInformation);

	if (bSucceeded)
	{
		CLogWriter::Log->info() << "Launch succeeded ";
		if (bCloseThis)
		{
			CLogWriter::Log->info() << "Exiting launcher after successful launch";
			TRACE("Launch completed.");
			exit(EXIT_SUCCESS);
		}
	}
	else
	{
		CLogWriter::Log->warn() << "Launch failed!";
		char msg[260];
		sprintf(msg, "Game launch failed.\nEither the game client executable \"KnightAscension.exe\" is missing, or the launcher application does not have enough privileges to launch the game.\nProceed with steps written below;\n1 - Check KnightAscension.exe exist in game folder.\n2 - Try to execute launcher with administrator privileges. To do so, right click on the \"Launcher.exe\" in the game folder(or desktop shortcut), and click on \"Run as Administrator\" menu item.\n(error code : %d)", GetLastError());
		
		::MessageBoxA(m_hWnd, msg, "Client launch failure!", MB_ICONWARNING);
		exit(EXIT_FAILURE);
	}
}

bool CMainWindowTranslucent::CheckUpdateLauncher()
{
	FILE * fp = fopen("Launcher2.exe", "r");
	if (fp)
	{
		CLogWriter::Log->info() << "Launcher update found, replacing launcher and deleting current";
		fclose(fp);
		rename("Launcher.exe", "Launcher.old");
		rename("Launcher2.exe", "Launcher.exe");
		LaunchExecutable(L"Launcher.exe", true);
	}
	return true;
}

void CMainWindowTranslucent::DownloadNextPatch()
{
	
	if (nullptr == m_pFTPClient)
	{
		TRACE("CMainWindowTranslucent::DownloadNextPatch >> FTP client is uninitialized!\n");
		return;
	}
	if (patchQueue.empty())
	{
		CLogWriter::Log->info() << "Update is completed";
		/* UPDATE COMPLETED */
		m_lblStatus.SetWindowTextW(L"Update completed.");
		m_progressBar.SetWindowTextW(L"");
		m_progressBar.SetRange(0, 100);
		m_progressBar.SetPos(100);
		m_btnStart.EnableWindow(true);
		return;
	}
	else
	{
		std::string nextPatch = patchQueue.front();
		const tstring wnextPatch = StdStringToWideString(nextPatch);
		/* Download asynchronously */
		/* Man, I fucking love lambda. */

		CLogWriter::Log->info() << "Downloading " << nextPatch;

		TRACE("CMainWindowTranslucent::DownloadNextPatch >> Downloading %s...\n", nextPatch.c_str());
		auto download_next([=]() {
			if (m_pFTPClient->DownloadFile(wnextPatch, wnextPatch, nsFTP::CRepresentation(nsFTP::CType::Image()), true))
			{
				TRACE("Download next patch >> File download succeeded");
				//CLogWriter::Log->info() << "Download of  " << WideStringToStdString(wnextPatch) << " is complete";
			}
		});
		WorkerThreadPool->enqueue(boost::bind<void>(download_next));
	}
}



void CMainWindowTranslucent::onConnectionFailure(void)
{
	CLogWriter::Log->warn() << "Failed to connect to the update server";
	m_lblStatus.SetWindowTextW(L"* Update server is not available right now *\n(or no internet connection)");
	TRACE("onConnectionFailure() >> Update server connection failed.");
}

void CMainWindowTranslucent::onResolveFailure(void)
{
	CLogWriter::Log->warn() << "Failed to resolve server host address";
	m_lblStatus.SetWindowTextW(L"Failed to resolve server address.\nPlease check your internet connection, and server IP address.");
}

void CMainWindowTranslucent::onConnected(void)
{
	CLogWriter::Log->info() << "Successfully established connection with update server";
	m_lblStatus.SetWindowTextW(L"Successfully connected to the update server.\nRequesting update information..");
}

void CMainWindowTranslucent::onDisconnected(void)
{
}



#pragma endregion

#pragma region ///	FTP Session

void CMainWindowTranslucent::OnInternalError(const tstring& strErrorMsg, const tstring& strFileName, DWORD dwLineNr)
{
	CLogWriter::Log->error("Internal FTP error occured.  Details : {}, in file {}, line {}\n",
		WideStringToStdString(strErrorMsg).c_str(),
		WideStringToStdString(strFileName).c_str(),
		dwLineNr);
	TRACE("FTP_OnInternalError() - Internal error occured. Details : %s, in file %s, line %d\n", 
		WideStringToStdString(strErrorMsg).c_str(), 
		WideStringToStdString(strFileName).c_str(), 
		dwLineNr);
}

void CMainWindowTranslucent::OnBeginReceivingData()
{
	TRACE("FTP_OnBeginReceivingData()\n");
	CLogWriter::Log->info("File transfer protocol data receive begin");
}

void CMainWindowTranslucent::OnEndReceivingData(long rSize)
{
	TRACE("FTP_OnEndReceivingData() - rSize %d byte(s)\n", rSize);
	CLogWriter::Log->info("File transfer protocol data receive ended, transfer size {} byte(s)", rSize);
}

void CMainWindowTranslucent::OnBytesReceived(const nsFTP::TByteVector &, long lReceivedBytes)
{
	m_UpdateInformation.CurrentFileDownloaded += lReceivedBytes;
	m_UpdateInformation.CalculateDownloadSpeed();

	m_progressBar.SetPos(m_UpdateInformation.CurrentFileDownloaded);
	char buf[255];

	int lower, upper;
	m_progressBar.GetRange(lower, upper);
	/* Percent */
	float percent = CalculatePercent(m_progressBar.GetPos(), upper);
	sprintf_s(buf, 255, progressbar_string, m_UpdateInformation.Filename.c_str(),
		(m_UpdateInformation.CurrentFileDownloaded / 1024),
		(m_UpdateInformation.CurrentFileSize / 1024),
		m_UpdateInformation.DownloadSpeed,
		percent);
	m_progressBar.SetWindowTextW(StdStringToWideString(std::string(buf)).c_str());
}

void CMainWindowTranslucent::OnReceiveFileResume(long lSeekOffset)
{
	CLogWriter::Log->info("Resuming file download from offset {}", lSeekOffset);
	m_UpdateInformation.CurrentFileDownloaded = lSeekOffset;
	m_UpdateInformation.CalculateDownloadSpeed();
	m_progressBar.SetPos(m_UpdateInformation.CurrentFileDownloaded);
	char buf[255];
	int lower, upper;
	m_progressBar.GetRange(lower, upper);
	/* Percent */
	float percent = CalculatePercent(m_progressBar.GetPos(), upper);
	sprintf_s(buf, 255, progressbar_string, m_UpdateInformation.Filename.c_str(),
		(m_UpdateInformation.CurrentFileDownloaded / 1024),
		(m_UpdateInformation.CurrentFileSize / 1024),
		m_UpdateInformation.DownloadSpeed,
		percent);
	m_progressBar.SetWindowTextW(StdStringToWideString(std::string(buf)).c_str());
}

void CMainWindowTranslucent::OnPreReceiveFile(const tstring & wstrSource, const tstring & wstrTarget, long lFileSize)
{
	CLogWriter::Log->info("[File transfer protocol][Pre-RF]  Source file : {}, Target file : {}, File size : {} byte(s)\n",
		WideStringToStdString(wstrSource).c_str(),
		WideStringToStdString(wstrTarget).c_str(),
		lFileSize);
	TRACE("FTP_OnPreReceiveFile() - Source file : %s, Target file : %s, File size : %d byte(s)\n", 
		WideStringToStdString(wstrSource).c_str(),
		WideStringToStdString(wstrTarget).c_str(), 
		lFileSize);

	m_UpdateInformation.UpdateFileInfo(WideStringToStdString(wstrSource), lFileSize);
	char label_buffer[255];
	sprintf_s(label_buffer,255, "Updating client [%d of %d update(s) installed]", m_UpdateInformation.DownloadedUpdates, m_UpdateInformation.TotalCountOfUpdates);
	m_lblStatus.SetWindowTextW(StdStringToWideString(std::string(label_buffer)).c_str());

	m_progressBar.SetRange(0, m_UpdateInformation.CurrentFileSize);
	m_progressBar.SetPos(0);

	char progressbar_buffer[255];
	int lower, upper;
	m_progressBar.GetRange(lower, upper);
	float percent = CalculatePercent(m_progressBar.GetPos(), upper);
	sprintf_s(progressbar_buffer, 255, progressbar_string, m_UpdateInformation.Filename.c_str(),
		(m_UpdateInformation.CurrentFileDownloaded / 1024),
		(m_UpdateInformation.CurrentFileSize / 1024),
		m_UpdateInformation.DownloadSpeed,
		percent);
	m_progressBar.SetWindowTextW(StdStringToWideString(std::string(progressbar_buffer)).c_str());
}

void CMainWindowTranslucent::OnPostReceiveFile(const tstring & wstrSource, const tstring & wstrTarget, long lFileSize)
{
	CLogWriter::Log->info("[File transfer protocol][Post-RF]  Source file : {}, Target file : {}, File size : {} byte(s)\n",
		WideStringToStdString(wstrSource).c_str(),
		WideStringToStdString(wstrTarget).c_str(),
		lFileSize);

    TRACE("FTP_OnPostReceiveFile() - Source file : %s, Target file : %s, File size : %d byte(s)\n",
	WideStringToStdString(wstrSource).c_str(),
	WideStringToStdString(wstrTarget).c_str(),
	lFileSize);
	
	try
	{
		CLogWriter::Log->info("Applying update file");
		m_lblStatus.SetWindowTextW(L"Applying update..");
		//m_progressBar.SetRange(0, 100);
		m_progressBar.SetPos(m_UpdateInformation.CurrentFileSize);
		m_progressBar.SetWindowTextW(L"Decompressing..");
		std::wstring fname = StdStringToWideString(m_UpdateInformation.Filename);
		wchar_t buf_file[MAX_PATH];
		_wgetcwd(buf_file, MAX_PATH);
		StrCatW(buf_file, L"\\");
		StrCatW(buf_file, fname.c_str());

		wchar_t buf[MAX_PATH];
		_wgetcwd(buf, MAX_PATH);
		StrCatW(buf, L"\\");

		/* Scoped */
		{
			SevenZip::SevenZipExtractor extractor(lib, std::wstring(buf_file));
			extractor.ExtractArchive(std::wstring(buf));
		}

		/* Decompress successful, pop the current and move to the next */

		patchQueue.pop();
		m_UpdateInformation.DownloadedUpdates++;
		/* Remove the decompressed file */
		StrCatW(buf, (PCWSTR)&fname);
		if (DeleteFileW(buf_file)) {
			CLogWriter::Log->info("Deleted update file");
			TRACE("FTP_OnPostReceiveFile() - Update file deleted.");
		}

		/* update1534.zip */
		/* text.length- 9, text.length - 5*/
		std::string strNewVersion = m_UpdateInformation.Filename.substr(m_UpdateInformation.Filename.length() - 7, 4);
		unsigned __int16 sNewVersion = atoi(strNewVersion.c_str());
		CUpdaterSession::WriteNewVersion(sNewVersion);
		/* If launcher is updated, update it.*/
		CheckUpdateLauncher();
		DownloadNextPatch();
	}
	catch (SevenZip::SevenZipException& e)
	{
		m_lblStatus.SetWindowTextW(L"Update file corrupted!\nRedownloading..");
		std::string res = WideStringToStdString(e.GetMessage());
		CLogWriter::Log->error("Decompression error : {}, redownloading the file..", res.c_str());
		TRACE("SevenZipExtract error %s, redownloading the file..", res.c_str());
		wchar_t buf[MAX_PATH];
		_wgetcwd(buf, MAX_PATH);
		StrCatW(buf, L"\\");
		const tstring fname = StdStringToWideString(m_UpdateInformation.Filename);
		StrCatW(buf, (PCWSTR)&fname);
		if (DeleteFileW(buf)) {
			CLogWriter::Log->info("Corrupt file deleted");
			TRACE("FTP_OnPostReceiveFile() - Update file deleted.");
		}
		DownloadNextPatch();
	}
}

void CMainWindowTranslucent::OnSendCommand(const nsFTP::CCommand & cmd, const nsFTP::CArg & arg)
{
	TRACE("FTP_OnSendCommand() >> %s\n", WideStringToStdString(cmd.AsString()).c_str());
	//CLogWriter::Log->error("Sent FTP command : {}", WideStringToStdString(cmd.AsString()).c_str());
}

void CMainWindowTranslucent::OnResponse(const nsFTP::CReply & rep)
{
	TRACE("FTP_OnResponse() >> %s\n", WideStringToStdString(rep.Value()).c_str());
	//CLogWriter::Log->error("Recv FTP command : {}", WideStringToStdString(rep.Value()).c_str());
}

#pragma endregion

/* START BUTTON */
void CMainWindowTranslucent::OnBnClickedButton3() noexcept
{
	LaunchExecutable(L"KnightAscension.exe", true);
}


void CMainWindowTranslucent::OnBnClickedButton4() noexcept
{
	const int result = ::MessageBoxA(m_hWnd, "Are you sure you want to cancel update and exit?", "Confirm", MB_YESNO | MB_ICONQUESTION);
	switch (result)
	{
	case IDYES:
		//exit(EXIT_SUCCESS);

		//delete this;
		AbortUpdateAndExit();
		break;
	}
}

/* Homepage button */
void CMainWindowTranslucent::OnBnClickedButton2() noexcept
{
	// TODO: Add your control notification handler code here
	
}

/* Options button */
void CMainWindowTranslucent::OnBnClickedButton1() noexcept
{
	COptionsDialog dialog(IDB_OPTIONS, L"PNG", 0, this);
	dialog.DoModal();
	// TODO: Add your control notification handler code here
}
