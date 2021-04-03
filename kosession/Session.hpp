/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */

/*

DISCLAIMER
This software can be used freely under GNU/LGPL license.

File    : Session.h
Author  : Mustafa K. GILOR
Purpose : Session class for the libtcp server.
All individual session info are stored in this class.
It is automatically destroyed when last shared_ptr instance is released.

*/

#pragma once

#pragma region Includes


#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/atomic.hpp>
#include <boost/noncopyable.hpp>




#include "CircularBuffer.hpp"
#include "SessionConfig.hpp"

using boost::asio::ip::tcp;


#pragma endregion


class Session : public boost::enable_shared_from_this<Session>, public boost::noncopyable
{
public:
	virtual ~Session()
	{
	}

	typedef boost::shared_ptr<Session> pointer;



	void Connect();
	void OnConnect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);

	/*
	Invoked when client is connected
	*/
	void OnConnect();

	/*
	Disconnects the session from connected endpoint.
	*/
	void Disconnect();
	/*
	Initializes the variables required for session.
	*/
	void Initialize();
	/*
	Keep alive system callback.
	*/
	void CheckAlive();

	/*
	Inheriting class should override this method by this signature :
	void OnDataReceived(char *, size_t) override;
	*/
	virtual void OnDataReceived(char *, size_t) = 0;
	virtual void OnConnected() = 0;
	virtual void OnDisconnected() = 0;
	virtual void OnStaticInitialized() = 0;
    virtual void OnConnectionFailed() = 0;

	/*
	Returns server's IP address
	*/
	std::string GetLocalIPAddress()   const     { return local_; }



	const char * GetLocalIPAddress_CSTR() const{ return local_.c_str(); }
	/*
	Returns IP address of connected client
	*/
	std::string GetRemoteIPAddress()  const     { return remote_; }
	/*
	Returns server's port
	*/
	unsigned short GetLocalPort()     const     { return localp_; }
	/*
	Returns connected client's port
	*/
	unsigned short GetRemotePort()    const     { return remotep_; }
	/*
	Returns the ID assigned to this session
	*/
	unsigned long long GetSessionID() const     { return m_ulSessionID; }

	void SetSessionID(unsigned long long id) { m_ulSessionID = id; }

	void SetAutokickPeriod(unsigned long long val){ m_ulSessionAutoKickPeriod = val; }
	/*
	Returns the socket object
	*/
	tcp::socket& socket()                       { return socket_; }

	pointer GetSharedFromThis() { return shared_from_this(); }

	void * Manager() const { return mgrPtr; }

protected:
	/*
	Default constructor (protected)
	*/
    Session(boost::asio::io_service& io_service,const std::string &server,const std::string &port,int ra,int ri) :
        socket_(io_service), resolver_(io_service), t_reconnect(io_service),fragment_count(0),
		isDisconnected(false), m_ulSessionID(0), szServerIP(server),
		serverp_(port),
		m_ulSessionEstablishTime(0),
		m_ulSessionAutoKickPeriod(0), reconnectAttempt(ra), reconnectInterval(ri)
	{
		if (!isStaticInitialized)
		{
			//OnStaticInitialized();
			isStaticInitialized = true;
		}
		m_readBuffer.Allocate(RECV_BUFFER_SIZE);
		m_readBuffer.Remove(RECV_BUFFER_SIZE);
	}

	/*
	Sends given buffer to the connected endpoint.
	*/
	void Send(void * buf, size_t len);
private:

	tcp::resolver resolver_;
	/*
	Socket
	*/
	tcp::socket socket_;
	/*
	Ring buffer for read operation
	*/
	CircularBuffer m_readBuffer;
	/*
	A mutex for the read event.
	It is claimed by any thread that completes a read event.
	Other pending read event should wait until current thread completes
	its' job.
	*/
	boost::recursive_mutex readMutex;

	boost::asio::deadline_timer t_reconnect;

	boost::atomic<int> reconnectAttempt;
	int reconnectInterval;


	/*
	Last data receive time.
	*/
	boost::atomic<unsigned long long> m_aulLastReceiveTime;
	/*
	Total received data amount (bytes)
	*/
	boost::atomic<unsigned long long> m_aulReceiveCount;
	/*
	Previous total received data amount (bytes)
	*/
	boost::atomic<unsigned long long> m_aulPrevReceiveCount;
	/*
	Maximum count of fragmented packet(s).
	If a session exceeds the MAX_PACKET_FRAGMENT_COUNT value
	it will be disconnected eventually.
	*/
	boost::atomic<int> fragment_count;
	/*
	Indicates if client is disconnected.
	*/
	boost::atomic<bool> isDisconnected;
	/*
	Identifier of this session.
	It is assigned by owner server and it should not be
	altered anywhere.(otherwise things might go crazy.)
	*/
	unsigned long long m_ulSessionID;

	/*
	The establish time of the session.
	(required for further features and autokick feature)
	*/
	unsigned long long m_ulSessionEstablishTime;


	/*
	The amount of time required before kicking this session.
	Default is 0 (meaning disabled).
	*/
	unsigned long long m_ulSessionAutoKickPeriod;
	/*
	Pointer to the owner server
	*/
	void * mgrPtr;
	/*
	Variables to store IP addresses
	*/
	std::string local_, remote_;
	std::string szServerIP, serverp_;
	/*
	Variables to store port values
	*/
	unsigned short localp_, remotep_;

	static  bool isStaticInitialized;

	void ReconnectWait();
	void OnReconnectWaitEnd();

	void OnResolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	/*
	Sets defined socket options for this session.
	*/
	void SetSocketOptions();
	/*
	Invoked when an asynchronous write operation is completed
	*/
	void OnWriteCompleted(const boost::system::error_code&, size_t s);
	/*
	Invoked when an asynchronous read operation is completed
	*/
	void OnReadCompleted(const boost::system::error_code& err, std::size_t bytes_transferred);

	/*
	Dispatches the parse event.
	*/
	void DispatchParse(size_t rSize, bool recursive = false);
	/*
	Issues another asynchronous read operation to fill the read buffer (if there's any data available.)
	*/
	void DispatchRead();
};




