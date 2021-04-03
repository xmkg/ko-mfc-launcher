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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "kosession\KOSession.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/make_shared.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include "kosession/IOServiceWorker.hpp"
#include <stack>

class CMainWindowTranslucent;

class CUpdaterSession final : public KOSession, public boost::enable_shared_from_this <CUpdaterSession>
{

    void onConnectionFailure(void);
    void onResolveFailure(void);
    void onConnected(void);
    void onDisconnected(void);
    void onPacketReceived(Packet* pkt);
public:
	void SetLauncherDlg(CMainWindowTranslucent * pLauncherDlg);
	
    static bool ReadConfiguration();
	static bool WriteNewVersion(const unsigned __int16 sNewVersion);
    static std::string m_strServerIP, m_strServerPort;
    static uint16 m_usLastVersion;
    static CIOServiceWorker io_service_worker_;
    static void RunIOService(){
        io_service_worker_.run();
    }
    static void StopIOService(){
        io_service_worker_.stop();
    }
	void abortSession() {
		
		this->Disconnect();
		StopIOService();
	}

    static boost::shared_ptr<CUpdaterSession> create()
	{
        return boost::make_shared<CUpdaterSession>(io_service_worker_.get_io_service(), CUpdaterSession::m_strServerIP, CUpdaterSession::m_strServerPort);
	}

    explicit CUpdaterSession(boost::asio::io_service& io_service,const std::string & serverIP, const std::string& sPort);
    CUpdaterSession(const CUpdaterSession& that) = delete;
    ~CUpdaterSession();

	bool HandlePacket(Packet& pkt) override;
	void OnSessionConnect()override;
	void OnSessionDisconnect() override;
    void OnSessionConnectFailed() override;

    void SendDownloadListReq(const uint16 sVersion);
private:
	CMainWindowTranslucent * m_pLauncherDlg;
	unsigned long long m_ulPrivateKey;
    unsigned __int64 m_ulPublicKey;
	short m_sSocketID;

	bool m_bDisconnected;

    void SendVersionReq();

	void HandleCompressed(Packet&pkt);

};

