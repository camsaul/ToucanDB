#pragma once

#include "http_parser_merged.h"

#include "datetime.h"
#include "parser.h"
#include "http_response.h"
#include "logging.h"
#include "settings.h"
#include "dumb_timer_queue.h"
#include "crow.h"

namespace crow
{
    using namespace boost;
    using tcp = asio::ip::tcp;
#ifdef CROW_ENABLE_DEBUG
    static int connectionCount;
#endif
    class Connection
    {
    public:
        Connection(boost::asio::io_service& io_service, Crow* handler);
        ~Connection();

        tcp::socket& socket() { return socket_; }

        void start();
        void handle_header();
        void handle();
        void complete_request();

    private:
        void do_read();
        void do_write();
        void check_destroy();
        void cancel_deadline_timer();
        void start_deadline(int timeout = 1);

    private:
        tcp::socket socket_;
        Crow* handler_;

        std::array<char, 4096> buffer_;

        HTTPParser<Connection> parser_;
        response res;

        bool close_connection_ = false;

        std::vector<boost::asio::const_buffer> buffers_;

        std::string content_length_;
        std::string date_str_;

        detail::dumb_timer_queue::key timer_cancel_key_;

        bool is_reading{};
        bool is_writing{};
    };

}
