#pragma once

#include <queue>

#include "parser.h"
#include "http_response.h"
#include "DumbTimerQueue.h"

#include "Handler.h"

namespace crow
{
    using namespace boost;
    using tcp = asio::ip::tcp;
    class Connection
    {
    public:
		Connection(boost::asio::io_service& io_service, Handler* handler);
        ~Connection();

        tcp::socket& socket() { return socket_; }

        void start();
        void handle();
        void complete_request();
		
		/// HTTP 1.1 Expect: 100-continue
		void HandleExpect100Continue();

    private:
        void do_read();
        void do_write();
        void check_destroy();
        void cancel_deadline_timer();
        void start_deadline(int timeout = 1);

        tcp::socket socket_;
        Handler* handler_;

        std::array<char, 4096> buffer_;

        HTTPParser parser_;
        response res;

        bool close_connection_ = false;

        std::vector<boost::asio::const_buffer> buffers_;

        detail::DumbTimerQueue::Key timer_cancel_key_;

        bool is_reading = false;
        bool is_writing = false;
    };

}
