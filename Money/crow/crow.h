#pragma once

#include "settings.h"
#include "utility.h"
#include "routing.h"

namespace crow
{
	class Server;
	
    class Crow
    {
    public:
        Crow() = default;

        void handle(const request& req, response& res);

        template <uint64_t Tag>
        auto route(std::string&& rule)
            -> typename std::result_of<decltype(&Router::new_rule_tagged<Tag>)(Router, std::string&&)>::type
        {
            return router_.new_rule_tagged<Tag>(std::move(rule));
        }

        Crow& port(std::uint16_t port)
        {
            port_ = port;
            return *this;
        }

        Crow& multithreaded()
        {
            return concurrency(std::thread::hardware_concurrency());
        }

        Crow& concurrency(std::uint16_t concurrency)
        {
            if (concurrency < 1)
                concurrency = 1;
            concurrency_ = concurrency;
            return *this;
        }

        void run();

        void debug_print();

    private:
        uint16_t port_ = 80;
        uint16_t concurrency_ = 1;

        Router router_;
    };
    using App = Crow;
};

