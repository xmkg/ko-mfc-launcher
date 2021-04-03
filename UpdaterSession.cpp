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

#include "kosession/lzf.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include "MainWindowTranslucent.h"
#include "LogWriter.h"

/* Static impl. */
std::string CUpdaterSession::m_strServerIP,
            CUpdaterSession::m_strServerPort;
uint16      CUpdaterSession::m_usLastVersion;

CIOServiceWorker CUpdaterSession::io_service_worker_(16);

    CUpdaterSession::CUpdaterSession(boost::asio::io_service& io_service,const std::string & serverIP, const std::string& sPort) :
    KOSession(io_service,serverIP,sPort)
    {
		
	}


    CUpdaterSession::~CUpdaterSession() {  }


    bool CUpdaterSession::ReadConfiguration()
    {
        boost::property_tree::ptree pt;
        // Load the XML file into the property tree. If reading fails
        // (cannot open file, parse error), an exception is thrown.
        try
        {
            read_xml(".//launcher.conf", pt);
        }
        catch (std::exception &ex)
        {
			//CLogWriter::Log->info("Loading 32-bit version of the decompression library");
			TRACE("ReadConfiguration >> Exception occured. Details : %s", ex.what());
            return false;
        }
		m_usLastVersion = pt.get("launcher.<xmlattr>.lastversion", 1534);
        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("launcher"))
        {
             if (v.first == "update_server")
             {
                auto update_server_entry = v.second;
                CUpdaterSession::m_strServerIP = v.second.get<std::string>("host", "149.202.223.2");
				CUpdaterSession::m_strServerPort = v.second.get<std::string>("port", "15100");
             }
             /* TODO : Support multiple update servers */
        }
          TRACE("Server IP : %s, Port : %s, Version %d\n",m_strServerIP.c_str(),m_strServerPort.c_str(),m_usLastVersion);
        return true;
    }

    void CUpdaterSession::OnSessionConnect() {
		m_pLauncherDlg->onConnected();
        SendVersionReq();
    }

    void CUpdaterSession::OnSessionDisconnect()
	{
		m_pLauncherDlg->onDisconnected();
		m_bDisconnected = true;
	}

    void CUpdaterSession::OnSessionConnectFailed()
    {
		m_pLauncherDlg->onConnectionFailure();
    }




    bool CUpdaterSession::HandlePacket(Packet&pkt)
	{
        /*Packet * recvRef = new Packet(pkt.GetOpcode());
        recvRef->append(pkt.contents(),pkt.size());*/
		m_pLauncherDlg->onPacketReceived(&pkt);
		return true;
    }

    void CUpdaterSession::HandleCompressed(Packet& pkt)
	{
		TRACE("CUserSession::HandleCompressed() - Compressed packet received");
		/* uint16 for 1298*/
		uint32 outLength = 0, inLength = 0;
		uint32 crc = 0;
	
		pkt >> outLength >> inLength >> crc;

		auto compBuffer = new uint8[outLength];
		auto decompressBuffer = new uint8[inLength];

		pkt.read(compBuffer, outLength);

		lzf_decompress(compBuffer, outLength, decompressBuffer, inLength);
		//0xedb88320
		//0
		auto checksum = crc32(decompressBuffer, inLength);
		if(checksum == crc)
		{
			TRACE("CUserSession::HandleCompressed() - CRC is valid.");
		}
	
		Packet decompressedPacket(decompressBuffer[0]);
		decompressedPacket.append(&decompressBuffer[1], inLength - 1);
		HandlePacket(decompressedPacket);
		delete[] decompressBuffer;
		delete[] compBuffer;
	}

    void CUpdaterSession::SendVersionReq()
    {
		CLogWriter::Log->info("Requesting version from server");
        Packet result(LS_VERSION_REQ);
        Send(&result);
    }

    void CUpdaterSession::SendDownloadListReq(const uint16 sVersion)
    {
		CLogWriter::Log->info("Requesting download list from server");
        Packet result(LS_DOWNLOADINFO_REQ);
        result << sVersion;
        Send(&result);
    }

	void CUpdaterSession::SetLauncherDlg(CMainWindowTranslucent * pLauncherDlg)
	{
		m_pLauncherDlg = pLauncherDlg;
	}

	bool CUpdaterSession::WriteNewVersion(const unsigned __int16 sNewVersion)
	{
		boost::property_tree::ptree pt;
		// Load the XML file into the property tree. If reading fails
		// (cannot open file, parse error), an exception is thrown.
		try
		{
			read_xml(".//launcher.conf", pt, boost::property_tree::xml_parser::trim_whitespace);
			pt.put("launcher.<xmlattr>.lastversion", sNewVersion);
			//boost::property_tree::xml_writer_settings<char> settings('\t', 1,"utf8");
			write_xml(".//launcher.conf", pt, std::locale());
		}
		catch (std::exception &ex)
		{
			TRACE("ReadConfiguration >> Exception occured. Details : %s", ex.what());
			return false;
		}



	}
