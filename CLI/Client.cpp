//
//  Client.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Client.h"
#include "Logging.h"

using boost::asio::ip::tcp;
using namespace toucan_db::logging;

namespace toucan_db {
	Client::Client(string host, int16_t port):
		host_		(host),
		port_		(port),
		ioService_	(make_unique<boost::asio::io_service>()),
		resolver_	(make_unique<tcp::resolver>(*ioService_)),
		query_		(host_, lexical_cast<string>(port_))
	{}
	
	Client::Client(Client&& rhs):
		host_		(std::move(rhs.host_)),
		port_		(rhs.port_),
		ioService_	(std::move(rhs.ioService_)),
		resolver_	(std::move(rhs.resolver_)),
		query_		(std::move(rhs.query_))
	{}
	
	void Client::Connect() {
		try {
			tcp::resolver::iterator endpoint_iterator = resolver_->resolve(query_);
			
			Logger(RED) << "Connecting to " << host_ << ", port " << port_ << "...";
			tcp::socket socket(*ioService_);
			boost::asio::connect(socket, endpoint_iterator);
			
			Logger(RED) << "Connected.";
			
			while (true) {
				boost::array<char, 128> buf;
				boost::system::error_code error;
				
				socket.read_some(boost::asio::buffer(buf), error);
				
				Logger(RED) << "Reading from server...";
				
				if (error == boost::asio::error::eof) {
					Logger(RED) << "Connection closed.";
					break; // Connection closed cleanly by peer.
				}
				else if (error) {
					throw boost::system::system_error(error); // Some other error.
				}
				
				Logger(RED) << "Read message from server: '" << buf.data() << '\'';
			}
			
		} catch (std::exception& e) {
			Logger(RED, cerr) << e.what();
		}
	}
}