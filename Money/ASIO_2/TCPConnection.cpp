//
//  TCPConnection.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "TCPConnection.h"

namespace toucan_db {
	shared_ptr<TCPConnection> TCPConnection::Create(boost::asio::io_service& io_service) {
		return shared_ptr<TCPConnection>(new TCPConnection(io_service));
	}
	
	void TCPConnection::Start() {
		static int i = 0;
		message_ = "MY MESSAGE " + lexical_cast<string>(i++); // TODO
		
		boost::asio::async_write(socket_, boost::asio::buffer(message_), boost::bind(&TCPConnection::HandleWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	
	void TCPConnection::HandleWrite(const boost::system::error_code& error, size_t bytesTransferred) {
		// nothing
	}
	
	TCPConnection::TCPConnection(boost::asio::io_service& io_service):
	socket_(io_service)
	{}
}