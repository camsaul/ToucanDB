//
//  TCPConnection.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "BasicConnection.h"

namespace toucan_db {
	class TCPConnection : public BasicConnection, public enable_shared_from_this<TCPConnection> {
	public:
		static shared_ptr<TCPConnection> Create(boost::asio::io_service& io_service);
		
		void Start();
		
//		void HandleRequest(const char* request);
		
	private:
		TCPConnection(boost::asio::io_service& io_service);
	};
}