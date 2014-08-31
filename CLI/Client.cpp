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
	atomic<int> Client::sReadsCount { 0 };
	
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
	
	static const int kNumBatchedRequests = 5;

	void Client::Connect() {
		while (sRequestsCount < kNumIterations) {
			for (int i = 0; i < kNumBatchedRequests; i++) {
				boost::asio::connect(socket_, endpoint_iterator_);
				boost::system::error_code error;
				while (true) {
					socket_.read_some(boost::asio::buffer(buffer_), error);
					sReadsCount++;
					if (error == boost::asio::error::eof) {
						sRequestsCount++;
						break;
					}
					else if (error) {
						auto e = boost::system::system_error(error);
						Logger(RED) << "Client::Connect(): error: " << e.what();
						throw e; // Some other error.
					}
				}
			}
		}
	}
	
	void Client::HandleConnect(const boost::system::error_code& error) {
	}
	
	void Client::Read() {
	}
	
	void Client::HandleRead(const boost::system::error_code& error) {
	}
}