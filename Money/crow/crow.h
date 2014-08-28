#pragma once

#include "settings.h"
#include "logging.h" 
#include "http_server.h"
#include "utility.h"
#include "routing.h"

namespace crow
{
    class Crow
    {
    public:
        using self_t = Crow;
        Crow()
        {
        }

        void handle(const request& req, response& res)
        {
			return router_.handle(req, res);
        }

        template <uint64_t Tag>
        auto route(std::string&& rule)
            -> typename std::result_of<decltype(&Router::new_rule_tagged<Tag>)(Router, std::string&&)>::type
        {
            return router_.new_rule_tagged<Tag>(std::move(rule));
        }

        self_t& port(std::uint16_t port)
        {
            port_ = port;
            return *this;
        }

        self_t& multithreaded()
        {
            return concurrency(std::thread::hardware_concurrency());
        }

        self_t& concurrency(std::uint16_t concurrency)
        {
            if (concurrency < 1)
                concurrency = 1;
            concurrency_ = concurrency;
            return *this;
        }

        void run()
        {
            Server<self_t> server(this, port_, concurrency_);
            server.run();
        }

        void debug_print()
        {
            CROW_LOG_DEBUG << "Routing:";
            router_.debug_print();
        }

    private:
        uint16_t port_ = 80;
        uint16_t concurrency_ = 1;

        Router router_;
    };
    using App = Crow;
};

