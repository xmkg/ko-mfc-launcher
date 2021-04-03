/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */

#include "Stdafx.h"
#include "KOSession.hpp"



KOSession::KOSession(boost::asio::io_service& io_service, const std::string& ip, const std::string& port) : Session(io_service, ip, port,0,5000), m_usingCrypto(false), m_sequence(0)
{

}


KOSession::~KOSession()
{
}

void KOSession::OnConnectionFailed()
{
    OnSessionConnectFailed();
}

void KOSession::OnStaticInitialized() {}

void KOSession::OnDataReceived(char * buf, size_t len)
{
	Packet pkt;
	if (DecryptData((unsigned char*)buf, len, pkt))
	{
		HandlePacket(pkt);
	}
	else
	{
		TRACE("KOSession::OnDataReceived() - Failed to decrypt data.");
		Disconnect();
	}
}


void KOSession::OnConnected()    { TRACE("KOSession::OnConnected() - New session connected");		OnSessionConnect(); }
void KOSession::OnDisconnected()
{
	TRACE("KOSession::OnDisconnected() - Session disconnected");
	OnSessionDisconnect();
}

void KOSession::EnableCrypto(unsigned long long pubKey,unsigned long long privateKey)
{
	m_crypto.SetPrivateKey(privateKey);
	m_crypto.SetPublicKey(pubKey);
	m_crypto.Init();
	m_usingCrypto = true;
}

bool KOSession::DecryptData(unsigned char *in_stream, size_t m_remaining, Packet & pkt)
{
	uint8* final_packet = nullptr;
	uint32 seqid = 0;

	if (isCryptoEnabled())
	{
		m_crypto.JvDecryptionFast(m_remaining, in_stream, in_stream);
		auto seq = *(uint16 *)(&in_stream[2]);
		auto chk = *(uint16 *)(in_stream);
		if (m_remaining < 4 /*|| m_sequence != seq*/ || *(uint16 *)(in_stream) !=chk)
			return false;

		m_remaining -= 5; // remove the sequence ID & CRC checksum
		final_packet = &in_stream[5];
	}
	else
		final_packet = in_stream;

	/* Discard opcode */
	m_remaining--;
	pkt = Packet(final_packet[0], (size_t)m_remaining);
	/* Append remaining */
	if (m_remaining > 0)
	{
		pkt.resize(m_remaining);
		memcpy((void*)pkt.contents(), &final_packet[1], m_remaining);
	}
	/* Else, the packet was only containing the opcode. */

	return true;
}

void KOSession::Send(Packet * pkt)
{
	unsigned char opcode = pkt->GetOpcode();
	unsigned char * out_stream = nullptr;
	unsigned __int16 len = (unsigned __int16)(pkt->size() + 1);
	int buffer_size = 0;
	switch (isCryptoEnabled())
	{
	case true:
	{
		/* CRC + Sequence length*/
		len += 4;
		/* CRC32 checksum at the end */
		len += 4;
		buffer_size = len + 6;
		
		out_stream = new unsigned char[buffer_size];
		memcpy(out_stream, (unsigned char*)(HEADER), 2);
		*(unsigned __int16 *)&out_stream[2] = len;
		*(unsigned __int32 *)&out_stream[4] = (unsigned __int32)(++m_sequence); // this isn't actually incremented here

		out_stream[8] = pkt->GetOpcode();

		if (pkt->size() > 0)
			memcpy(&out_stream[9], pkt->contents(), pkt->size());
		auto chksum = crc32(&out_stream[4], len-4, -1);
		memcpy(&out_stream[buffer_size - 6], &chksum, 4);
		memcpy(&out_stream[buffer_size - 2], (unsigned char*)(TAIL), 2);

		m_crypto.JvEncryptionFastBetweenIndex(4, len, out_stream, out_stream);
	
	}
	break;
	case false:
	{
		buffer_size = len + 6;
		out_stream = new unsigned char[buffer_size];
		memset(out_stream, 0, buffer_size);
		memcpy(&out_stream[0], (unsigned char*)(HEADER), 2);
		memcpy(&out_stream[2], &len, 2);
		*(unsigned __int16 *)&out_stream[2] = len;
		/* AA 55 00 00 01*/
		out_stream[4] = pkt->GetOpcode();
		if (pkt->size() > 0)
			memcpy(&out_stream[5], pkt->contents(), pkt->size());
		memcpy(&out_stream[buffer_size - 2], (unsigned char*)(TAIL), 2);
	}
	break;
	}

	Session::Send(out_stream, buffer_size);
	delete[] out_stream;
}

bool KOSession::SendCompressed(Packet * pkt)
{
	if (pkt->size() < 500)
	{
		Send(pkt);
		return true;
	}

	Packet result(WIZ_COMPRESS_PACKET);
	uint32 inLength = uint32(pkt->size() + 1), outLength = inLength + LZF_MARGIN, crc;
	uint8 *buffer = new uint8[inLength], *outBuffer = new uint8[outLength];

	*buffer = pkt->GetOpcode();
	if (pkt->size() > 0)
		memcpy(buffer + 1, pkt->contents(), pkt->size());

	crc = (uint32)crc32(buffer, inLength);
	outLength = lzf_compress(buffer, inLength, outBuffer, outLength);

#if __VERSION >= 1453 // 32-bit
	result << outLength << inLength;
#else // 16-bit
	result << uint16(outLength) << uint16(inLength);
#endif
	result << uint32(crc);

	result.append(outBuffer, outLength);

	delete[] buffer;
	delete[] outBuffer;

	Send(&result);
	return true;
}
