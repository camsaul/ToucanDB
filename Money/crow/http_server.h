#pragma once

#include "http_connection.h"
#include "DumbTimerQueue.h"

namespace crow
{
    using namespace boost;
    using tcp = asio::ip::tcp;
	
	class Crow;
    
    class Server
    {
    public:
        Server(Handler* handler, uint16_t port, uint16_t concurrency);

        void run();
        void stop();

    private:
        asio::io_service& pick_io_service();

        void do_accept();

    private:
        asio::io_service io_service_;
        vector<unique_ptr<asio::io_service>> io_service_pool_;
        tcp::acceptor acceptor_;
        boost::asio::signal_set signals_;

        Handler* handler_;
        uint16_t concurrency_{1};
        uint16_t port_;
        std::atomic<unsigned> roundrobin_index_ {0};
    };
}
