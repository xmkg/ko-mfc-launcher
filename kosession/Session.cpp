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
#include "Session.hpp"
#include <string>



bool Session::isStaticInitialized = false;



std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}



void Session::Initialize()
{

	SetSocketOptions();

	local_ = socket_.local_endpoint().address().to_string();
	remote_ = socket_.remote_endpoint().address().to_string();
	localp_ = socket_.local_endpoint().port();
	remotep_ = socket_.remote_endpoint().port();

    //m_aulLastReceiveTime.store(GetTickCount64());
	m_aulReceiveCount.store(0);
	m_aulPrevReceiveCount.store(0);

	/* Generate ID for this session */
	/*char buf[256];

	std::string withoutDots = ReplaceString(remote_, ".", "");
	sprintf_s(buf, "%s%hu", withoutDots.c_str(), remotep_);

	m_ulSessionID = _atoi64(buf);*/

}

void Session::SetSocketOptions()
{
	socket_.set_option(boost::asio::socket_base::send_buffer_size(INTERNAL_SEND_BUFFER_SIZE));
	socket_.set_option(boost::asio::socket_base::receive_buffer_size(INTERNAL_RECV_BUFFER_SIZE));

#ifdef USE_KEEP_ALIVE
	socket_.set_option(boost::asio::socket_base::keep_alive(true));
#endif

#ifdef USE_TCP_NODELAY
	socket_.set_option(tcp::no_delay(true));
#endif
	socket_.set_option(boost::asio::socket_base::linger(false, 0));
}

//#ifdef LIBTCP_CLIENT
void Session::Connect()
{
	if (szServerIP.empty())
	{
		OnConnectionFailed();
		return;
	}
	tcp::resolver::query query(tcp::v4(), const_cast<char*>(szServerIP.c_str()), serverp_);
	tcp::resolver::iterator iterator = resolver_.resolve(query);
    TRACE("Resolving %s:%s",szServerIP.c_str(),serverp_.c_str());

	resolver_.async_resolve(query,
		boost::bind(&Session::OnResolve, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::iterator));
}

void Session::ReconnectWait()
{
    TRACE("Reconnect wait");
	/*
	If we're running out of attempts, we should disconnect.
	*/
	if (--reconnectAttempt == 0)
		Disconnect();

	/*
	Set the expiration time for reconnect timer
	*/
	t_reconnect.expires_from_now(boost::posix_time::milliseconds(reconnectInterval));
	/*
	Wait until reconnect interval expires
	*/
	t_reconnect.async_wait(boost::bind(&Session::OnReconnectWaitEnd, shared_from_this()));
}

void Session::OnReconnectWaitEnd()
{
	/* Call connect again */
	Connect();
    TRACE("Reconnect wait end");
}

void Session::OnConnect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
    TRACE("OnConnect invoked");
	if (!err)
	{
        TRACE("Connected!");
		/*
		We're not disconnected anymore.
		Set the flag.
		*/
		isDisconnected = false;
		// Connection was successful.
		Initialize();
		OnConnected();

		/*
		Dispatch the read operation
		*/
		DispatchRead();

	}
	else if (endpoint_iterator != tcp::resolver::iterator())
	{
		// The connection failed. Try the next endpoint in the list.
		socket_.close();
		tcp::endpoint endpoint = *endpoint_iterator;
		socket_.async_connect(endpoint,
			boost::bind(&Session::OnConnect, shared_from_this(),
			boost::asio::placeholders::error, ++endpoint_iterator));
	}
	else
	{

		/*
		Check if automatic reconnect is enabled.
		If so, call the reconnect wait function to try connecting again.
		*/
		if (reconnectAttempt > 0)
			ReconnectWait();
		/*
		Otherwise, remove this client from manager, and free the memory.
		*/
		else
        {
            TRACE("Connection failure");
            OnConnectionFailed();
			Disconnect();
        }

	}

}



void Session::OnResolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
	if (!err)
	{
		/*
		Fetch the first endpoint we've just resolved.
		*/
		tcp::endpoint endpoint = *endpoint_iterator;
        TRACE("IP resolved as %s",endpoint.address().to_string().c_str());
		/*
		Try to connect to the endpoint.

		NOTE: If we've got more than one endpoint, the others will be tried next
		if current endpoint fails to connect.
		*/
		socket_.async_connect(endpoint, boost::bind(&Session::OnConnect, shared_from_this(),
			boost::asio::placeholders::error,
			++endpoint_iterator));

	}
	else
	{
        TRACE("[onresolve]Error: %s ",err.message().c_str());
	}
	return;
}



void Session::CheckAlive()
{

}

/*
Sends given buffer to the connected endpoint.
@params
void * buf			The buffer to send.
size_t len			The length of the buffer.
*/
void Session::Send(void * buf, size_t len)
{
	if (isDisconnected)
		return;
	boost::asio::async_write
		(
		socket_,
		boost::asio::buffer(buf, len),
		// Bind callback function
		boost::bind
		(
		&Session::OnWriteCompleted,
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred)
		);
}



void Session::DispatchRead()
{
	if (isDisconnected)
		return;

	++m_aulReceiveCount;

	boost::asio::async_read
		(
		// Socket to read from
		socket_,
		// Buffer
		boost::asio::buffer(m_readBuffer.GetBuffer(), m_readBuffer.GetSpace()),
		// Minimum transfer amount
		boost::asio::transfer_at_least(MIN_RECV_TRANSFER_AMOUNT),
		boost::bind
		(
		&Session::OnReadCompleted,
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred
		)
		);
}

#pragma warning(suppress: 6262)
void Session::DispatchParse(size_t rSize, bool recursive)
{
	unsigned char first_chunk[2 + SIZE_TYPE];
	unsigned char head[2], tail[2], tmpSize[SIZE_TYPE];
	char core[MAX_INDIVIDUAL_PACKET_SIZE];
	bool bFoundTail = false;
	__int64 size = 0;

	do
	{

		/* Check if we have enough byte(s) to issue a read operation and we're connected. */
		if ((m_readBuffer.GetSize() < MIN_RECV_TRANSFER_AMOUNT) || isDisconnected)
			return;



		memcpy(&first_chunk, m_readBuffer.GetBufferStart(), 2 + SIZE_TYPE);
		memcpy(&head, &first_chunk, 2);
		memcpy(&tmpSize, &first_chunk[2], SIZE_TYPE);
        static char p_header[] = {0xAA,0x55};
		// Check the packet header
        if (memcmp(head, reinterpret_cast<unsigned char*>(p_header), 2) == 0)
		{
			// Read the size of the content

			switch (SIZE_TYPE)
			{
			case SIZETYPE_WORD:
				size = reinterpret_cast<__int16*>(tmpSize)[0];
				break;
			case SIZETYPE_DWORD:
				size = reinterpret_cast<__int32*>(tmpSize)[0];
				break;
			case SIZETYPE_QWORD:
				size = reinterpret_cast<__int64*>(tmpSize)[0];
				break;
			}

			// Confirm that content size + header + 4 + tail matches received amount.
			// Confirm that size is not minus.
			if (size <= 0 || size > MAX_INDIVIDUAL_PACKET_SIZE)
			{
				// That means something's wrong with this packet.
				// We should disconnect this session.
                TRACE( "Session::DispatchParse()\n	either content size is zero or exceeds the maximum amount.\n	disconnecting.." ,size);
				goto Failure;
			}

			if (m_readBuffer.GetSize() >= static_cast<unsigned int>(4 + SIZE_TYPE + size))
			{
				m_readBuffer.Remove(2 + SIZE_TYPE);
				// Read the message to the new allocated buffer
				m_readBuffer.Read(&core, size);
				// Read the tail
				m_readBuffer.Read(reinterpret_cast<char*>(tail), 2);
                static char p_tail[] = {0x55,0xAA};
                if (memcmp(tail, reinterpret_cast<unsigned char*>(p_tail), 2) == 0)
				{
					// Valid packet.
					// Trigger packet receive function of child class
					OnDataReceived(&core[0], size);

					bFoundTail = true;
				} // EOF tail check
				else
				{
                    TRACE( "Session::DispatchParse()\n	Tail mismatch.\n	disconnecting..");
					goto Failure;
				}

			} // END OF m_readBuffer.GetSize() >= (unsigned int)(4 + SIZE_TYPE + size)
			else
			{
				/*
				@ Mustafa, 02.07.2015

				Else, it means we don't have the whole data yet.(fragmented packet)
				We should issue another read to receive remaining data.
				But, it should not happen frequently.
				If it does, it is probably an attack(DDoS) or another harmful action.

				We should kick out the session if fragmentation count exceeds the determined
				threshold.

				*/
				if (++fragment_count == MAX_PACKET_FRAGMENT_COUNT)
				{
                    TRACE( "Session::DispatchParse()\n	fragmentation level exceeded maximum value.\n	disconnecting..");
					goto Failure;
				}
				else
				{
					TRACE("Session::DispatchParse() - Fragmented packet.");
					return;
				}
			}
		}
		else
		{
            TRACE( "Session::DispatchParse()\n	Header mismatch.\n	disconnecting..");
			goto Failure;
		}
	} while (bFoundTail);


Failure:
	Disconnect();
}

void Session::OnConnect()
{
    TRACE("OnConnect invoked");
	// Start reading..
	DispatchRead();
	/*
	Call the virtual function to inform inherited class
	*/
	OnConnected();
}



void Session::OnWriteCompleted(const boost::system::error_code&, size_t s)
{

}

void Session::OnReadCompleted(const boost::system::error_code& err, size_t s)
{
	// scoped lock
	{
		boost::unique_lock<boost::recursive_mutex> scoped_lock(readMutex);

		switch (err.value())
		{
		case ERROR_SUCCESS:
		case boost::asio::error::in_progress:
			break;
		case boost::asio::error::operation_aborted:
			return;
		default:
			goto disco;
		}

		// Commit the data
		m_readBuffer.IncrementWritten(s);

		// Parse incoming data
		DispatchParse(s);

		// Update the recv amount..
		//((Server<Session>*)serverPtr)->OnReceive(s);

		// Dispatch the read event again
		if (!isDisconnected)
			DispatchRead();
	}
	return;
disco:
	Disconnect();
}

void Session::Disconnect()
{
	if (isDisconnected)
		return;
	isDisconnected = true;
	//socket_.shutdown(tcp::socket::shutdown_both);
	socket_.close();
	/*
	Call the virtual function.
	*/
	OnDisconnected();
}



