//
//  AsyncServer.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
	class TCPConnection;
	
	namespace server {
		using boost::asio::ip::tcp;
		
		static const uint16_t kDefaultPort = 1337;
				
		
		class AsyncServer {
		public:
			class ConfigurationBuilder : boost::noncopyable {
			public:
				friend class AsyncServer;
				ConfigurationBuilder(ConfigurationBuilder&&);
				~ConfigurationBuilder(); ///< start the server when the builder gets dealloced
				
				ConfigurationBuilder& Headless			(bool makeHeadless);
				ConfigurationBuilder& NumberOfThreads	(uint16_t numThreads);
				ConfigurationBuilder& Port				(uint16_t port);
			private:
				ConfigurationBuilder() = default;
				
				bool startsServerUponDestruction_ = true; ///< Only one builder should do it 
				bool headless_ = false;
				uint16_t numThreads_ = 1;
				uint16_t port_ = kDefaultPort;
			};
			
			static AsyncServer::ConfigurationBuilder Start();
		
			AsyncServer(uint16_t port = kDefaultPort);
			
			void StartAccept();
			void HandleAccept(shared_ptr<TCPConnection> newConnection, const boost::system::error_code& error);
			
		private:
			unique_ptr<boost::asio::io_service> ioService_;
			tcp::acceptor acceptor_;
		};

	}
}