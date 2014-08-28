#pragma once

#include "settings.h"
#include "utility.h"
#include "routing.h"

namespace crow
{
	#define CROW_ROUTE(app, url) app.route<crow::black_magic::get_parameter_tag(url)>(url)
	
	class Server;
    class Crow
    {
    public:
        Crow(uint16_t port = 1337):
			port_(port),
			concurrency_(std::thread::hardware_concurrency())
		{}

        void handle(const request& req, response& res);

        template <uint64_t Tag>
        auto route(std::string&& rule)
            -> typename std::result_of<decltype(&Router::new_rule_tagged<Tag>)(Router, std::string&&)>::type
        {
            return router_.new_rule_tagged<Tag>(std::move(rule));
        }

        void run();
        void debug_print();

    private:
        uint16_t port_;
        uint16_t concurrency_;

        Router router_;
    };
    using App = Crow;
};

