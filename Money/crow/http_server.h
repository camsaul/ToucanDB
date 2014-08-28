#pragma once

#include "http_connection.h"
#include "datetime.h"
#include "logging.h"
#include "dumb_timer_queue.h"

namespace crow
{
    using namespace boost;
    using tcp = asio::ip::tcp;
	
	class Crow;
    
    class Server
    {
    public:
        Server(Crow* handler, uint16_t port, uint16_t concurrency = 1):
			acceptor_(io_service_, tcp::endpoint(asio::ip::address(), port)),
            signals_(io_service_, SIGINT, SIGTERM),
            handler_(handler), 
            concurrency_(concurrency),
            port_(port)
        {}

        void run();
        void stop();

    private:
        asio::io_service& pick_io_service();

        void do_accept();

    private:
        asio::io_service io_service_;
        std::vector<std::unique_ptr<asio::io_service>> io_service_pool_;
        tcp::acceptor acceptor_;
        boost::asio::signal_set signals_;

        Crow* handler_;
        uint16_t concurrency_{1};
        uint16_t port_;
        unsigned int roundrobin_index_{};
    };
}
