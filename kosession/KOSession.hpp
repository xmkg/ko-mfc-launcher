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
/*
A template inherited class.
*/
#include "Session.hpp"
#include "Packet.hpp"
#include "JvCryption.h"
#include "lzf.h"
#include "packets.h"
#include <boost/asio/io_service.hpp>

// KO sockets time out after at least 30 seconds of inactivity.
#define KOSOCKET_TIMEOUT (30) 

// Allow up to 30 minutes for the player to create their character / the client to load.
#define KOSOCKET_LOADING_TIMEOUT (30 * 60)

class KOSession : public Session
{
public:

	virtual bool HandlePacket(Packet & pkt) = 0;
	virtual void OnSessionConnect() = 0;
	virtual void OnSessionDisconnect() = 0;
    virtual void OnSessionConnectFailed() = 0;
	__inline bool isCryptoEnabled() { return m_usingCrypto; };
	__inline unsigned __int16 GetSocketID() { return static_cast<__int16>(GetSessionID()); };
	void OnDataReceived(char *, size_t) override;
	void OnConnected() override;
	void OnDisconnected() override;
	void OnStaticInitialized() override;
    void OnConnectionFailed() override;
	void EnableCrypto(unsigned long long publicKey, unsigned long long privateKey);
	void Send(Packet * pkt);
	bool SendCompressed(Packet * pkt);
    KOSession(boost::asio::io_service& io_service, const std::string&ip, const std::string&port);
	~KOSession();
private:
	bool DecryptData(unsigned char * in_stream, size_t len, Packet & pkt);
	CJvCryption m_crypto;
	uint32 m_sequence;
	unsigned short m_socketID;
	bool m_usingCrypto;

};



