#include "crow.h"
#include "http_server.h"
#include "logging.h"

namespace crow {
	void Crow::handle(const request& req, response& res)
	{
		return router_.handle(req, res);
	}
	
	void Crow::run()
	{
		Server server(this, port_, concurrency_);
		server.run();
	}
	
	void Crow::debug_print()
	{
		CROW_LOG_DEBUG << "Routing:";
		router_.debug_print();
	}
}