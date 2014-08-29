//
//  SingleThreadedServer.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Logging.h"
#include "SingleThreadedServer.h"

using boost::asio::ip::tcp;
using namespace toucan_db::logging;

namespace toucan_db {
	namespace server {
		void SingleThreadedServer::Start() {
			try {
				boost::asio::io_service io_service;
				tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1337));
				
				Logger(GREEN) << "Starting server on port 1337...";
				
				while (true) {
					tcp::socket socket(io_service);
					acceptor.accept(socket);
					
					Logger(GREEN) << "Accepted connection, writing message to +-*9...";
					
					std::string message = "COOL!";
					
					boost::system::error_code ignored_error;
					boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
					
					Logger(GREEN) << "Finished writing message.";
				}
				
			} catch (exception& e) {
				Logger(GREEN, cerr) << e.what();
			}
		}
	}
}