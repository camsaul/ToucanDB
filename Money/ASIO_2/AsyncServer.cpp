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
		atomic<int> AsyncServer::sNumberOfConnectionsAccepted { 0 };
		
		using Builder = toucan_db::server::AsyncServer::ConfigurationBuilder;
	
		Builder::ConfigurationBuilder(Builder&& rhs):
			headless_ (rhs.headless_)
		{
			if (&rhs != this) {
				rhs.startsServerUponDestruction_ = false;
			}
		}

		Builder& Builder::Headless(bool headless) {
			headless_ = headless;
			return *this;
		}
		
		Builder& Builder::NumberOfThreads(uint16_t numThreads) {
			numThreads_ = numThreads;
			return *this;
		}
		
		Builder& Builder::Port(uint16_t port) {
			port_ = port;
			return *this;
		}

		Builder::~ConfigurationBuilder() {
			if (!startsServerUponDestruction_) return;
			
			
			auto startServerFn = [numThreads = numThreads_, port = port_]{
				try {
					Logger(BLUE) << "Starting async server on port " << port << "...";

					AsyncServer server(port);
					boost::asio::io_service* ptr = server.ioService_.get();
					Logger(RED) << "Created new io_service instance: " << hex << server.ioService_.get() << dec;
					
					// start an extra io_service thread for every thread > 1
					for (int i = 1; i < numThreads; i++) {
						Logger(BLUE) << "port " << port << ": starting additional io_service thread #" << i;
						thread t ([ioService=ptr]{
							ioService->run();
						});
						t.detach();
					}
										
					server.ioService_->run();
				} catch (exception& e) {
					Logger(BLUE) << "Caught exception: " << e.what();
				}
			};
			
			if (headless_) {
				auto t = thread(startServerFn);
				t.detach();
			} else {
				startServerFn();
			}
		}
		
		Builder AsyncServer::Start() {
			return std::move(Builder());
		}
		
		AsyncServer::AsyncServer(uint16_t port):
			ioService_	(new boost::asio::io_service()),
			acceptor_	(*ioService_, tcp::endpoint(tcp::v4(), port))
		{
			StartAccept();
		}
		
		void AsyncServer::StartAccept() {
			auto newConnection = TCPConnection::Create(acceptor_.get_io_service());
			acceptor_.async_accept(newConnection->Socket(), bind(&AsyncServer::HandleAccept, this, newConnection, boost::asio::placeholders::error));
		}
		
		void AsyncServer::HandleAccept(shared_ptr<TCPConnection> newConnection, const boost::system::error_code& error) {
			if (!error) {
				sNumberOfConnectionsAccepted++;
				newConnection->Start();
			} else if (error) {
				auto e = boost::system::system_error(error);
				Logger(RED) << "AsyncServer::HandleAccept() error: " << e.what();
				throw e;
			}
			
			StartAccept();
		}
	}
}