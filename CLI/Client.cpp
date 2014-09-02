//
//  Client.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Client.h"
#include "Logging.h"

using namespace toucan_db::logging;

namespace toucan_db {
	atomic<int> Client::sRequestsCount { 0 };
	
	shared_ptr<Client> Client::Create(string host, int16_t port) {
		return shared_ptr<Client>(new Client(host, port));
	}
	
	Client::Client(string host, int16_t port):
		host_				(host),
		port_				(port)
	{
		boost::asio::io_service ioService;
		SetSocket(ioService);
		tcp::resolver			resolver			{ ioService };
		tcp::resolver::query	query				{ host_, lexical_cast<string>(port_) };
		auto					endpointIterator	= resolver.resolve(query);
		auto					socket				= make_unique<tcp::socket>(ioService);

		boost::asio::connect(Socket(), endpointIterator);
	}
	
	const char* Client::Request(const char* msg) {
		WriteSync(msg);
		return Read();
	}
}