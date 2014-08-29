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
		
		class AsyncServer {
		public:
			static void Start(bool headless=false);
		
			AsyncServer(boost::asio::io_service& io_service);
			
			void StartAccept();
			void HandleAccept(shared_ptr<TCPConnection> newConnection, const boost::system::error_code& error);
			
		private:
			tcp::acceptor acceptor_;
		};

	}
}