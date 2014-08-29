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
	atomic<int> Client::sRequestsCount { 0 };
	
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
	
//	void Client::Connect(int iterations) {
//		numIterations_ = iterations;
//		Connect();
//	}

	void Client::Connect() {
		if (sRequestsCount == 0) return;
//		if (numIterations_ <= 0) return;
//		
		Logger(GREEN) << "Connecting to " << host_ << ":" << port_ << "...";
		try {
			Logger(BLUE) << "1";
			boost::asio::async_connect(socket_, endpoint_iterator_, bind(&Client::HandleConnect, shared_from_this(), boost::asio::placeholders::error));
		} catch (std::exception& e) {
			Logger(GREEN, cerr) << e.what();
		}
		Logger(BLUE) << "2";
	}
	
	void Client::HandleConnect(const boost::system::error_code& error) {
		Logger(BLUE) << "Client::HandleConnect()";
		
		if (error) {
			Logger(RED) << "ERROR: " << error;
			return;
		}
		Read();
	}
	
	void Client::Read() {
		try {
			socket_.async_read_some(boost::asio::buffer(buffer_), boost::bind(&Client::HandleRead, shared_from_this(), boost::asio::placeholders::error));
		} catch (exception& e) {
			Logger(RED) << "Caught exception in Client::Read(): " << e.what();
		}
	}
	
	void Client::HandleRead(const boost::system::error_code& error) {
		if (error == boost::asio::error::eof) {
			sRequestsCount--;
			// connection closed, just go back to connecting
			return Connect();
		} else {
			Logger(RED) << "ERROR: " << error;
			return;
		}
		
		Logger(ORANGE) << "Read message from server: '" << buffer_.data() << "'";
		
		Read();
	}
}