//
//  AsyncServer.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "AsyncServer.h"
#include "TCPConnection.h"
#include "Logging.h"

using namespace toucan_db::logging;

namespace toucan_db {
	namespace server {
		void AsyncServer::Start(bool headless) {
			if (headless) {
				auto t = thread(AsyncServer::Start, false); // detach thread which will call this fn again
				t.detach();
				return;
			}
			try {
				Logger(BLUE) << "Starting async server...";
				boost::asio::io_service io_service;
				AsyncServer server(io_service);
				io_service.run();
			} catch (exception& e) {
				Logger(BLUE) << "Caught exception: " << e.what();
			}
		}
		
		AsyncServer::AsyncServer(boost::asio::io_service& io_service, u_int16t port):
			acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
		{
			StartAccept();
		}
		
		void AsyncServer::StartAccept() {
			auto newConnection = TCPConnection::Create(acceptor_.get_io_service());
			acceptor_.async_accept(newConnection->Socket(), bind(&AsyncServer::HandleAccept, this, newConnection, boost::asio::placeholders::error));
		}
		
		void AsyncServer::HandleAccept(shared_ptr<TCPConnection> newConnection, const boost::system::error_code& error) {
			if (!error) {
				newConnection->Start();
			}
			
			StartAccept();
		}
	}
}