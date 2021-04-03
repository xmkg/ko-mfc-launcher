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

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;
class CIOServiceWorker : private boost::noncopyable
{
public:
	explicit CIOServiceWorker(const unsigned __int32 nWorkerThreadCount):_ioservice(nWorkerThreadCount),_work(_ioservice),_workerCount(nWorkerThreadCount){}

	void run()
	{
		// Create a pool of threads to run all of the io_services.
		std::vector<boost::shared_ptr<boost::thread> > threads;
		for (std::size_t n = 0; n < _workerCount; n++)
		{
			boost::shared_ptr<boost::thread> thread(new boost::thread(
				boost::bind(&boost::asio::io_service::run, &_ioservice)));

			// Set the priority
			SetThreadPriority(thread->native_handle(), THREAD_PRIORITY_HIGHEST);

			threads.push_back(thread);
		}

		// Wait for all threads in the pool to exit.
		for (std::size_t i = 0; i < threads.size(); ++i)
            threads [i]->detach();
	}

	void stop()
	{
		_ioservice.stop();
	}

	io_service & get_io_service() { return _ioservice; }
private:
	io_service _ioservice;
	/* This will keep the threads busy when idle. (to prevent exit)*/
	io_service::work _work;

	unsigned __int32 _workerCount = { 0 };
};
