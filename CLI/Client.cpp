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
		host_				(host),
		port_				(port),
		ioService_			(make_unique<boost::asio::io_service>()),
		resolver_			(make_unique<tcp::resolver>(*ioService_)),
		query_				(host_, lexical_cast<string>(port_)),
		endpoint_iterator_	(resolver_->resolve(query_)),
		socket_				(*ioService_)
	{}
	
	Client::Client(Client&& rhs):
		host_				(std::move(rhs.host_)),
		port_				(rhs.port_),
		ioService_			(std::move(rhs.ioService_)),
		resolver_			(std::move(rhs.resolver_)),
		query_				(std::move(rhs.query_)),
		endpoint_iterator_	(std::move(rhs.endpoint_iterator_)),
		socket_				(std::move(rhs.socket_))
	{}
	
	void Client::Connect(size_t iterations) {
		try {
			Logger(GREEN) << "Connected to " << host_ << ":" << port_ << "...";
			
			for (int i = 0; i < iterations; i++ ) {
				boost::asio::connect(socket_, endpoint_iterator_);
			
				while (true) {
					boost::system::error_code error;
					
					socket_.read_some(boost::asio::buffer(buffer_), error);
					
					if (error == boost::asio::error::eof) {
//						Logger(GREEN) << "Connection closed.";
						break; // Connection closed cleanly by peer.
					}
//					else if (error) {
//						throw boost::system::system_error(error); // Some other error.
//					}
					
//					Logger(RED) << "Read message from server: '" << buf.data() << "'";
				}
			}
			
		} catch (std::exception& e) {
			Logger(GREEN, cerr) << e.what();
		}
	}
}