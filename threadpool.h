/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


#ifndef THREAD_POOL_CLASS
#define THREAD_POOL_CLASS

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

typedef std::unique_ptr<boost::asio::io_service::work> asio_worker;

struct ThreadPool
{
    explicit ThreadPool(size_t threads) :service(), working(new asio_worker::element_type(service))
    {
        while (threads--)
        {
            auto worker = boost::bind(&boost::asio::io_service::run, &(this->service));
            g.add_thread(new boost::thread(worker));
        }
    }

    template<class F>
    void enqueue(F f)
    {
        service.post(f);
    }

    ~ThreadPool()
    {
        working.reset(); //allow run() to exit
        g.join_all();
        service.stop();
    }

private:
    boost::asio::io_service service; //< the io_service we are wrapping
    asio_worker working;
    boost::thread_group g; //< need to keep track of threads so we can join them
};
#endif
