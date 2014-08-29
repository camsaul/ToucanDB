//
//  TCPConnection.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "TCPConnection.h"
#include "Storage.h"
#include "Logging.h"

using namespace toucan_db::logging;

namespace toucan_db {
	shared_ptr<TCPConnection> TCPConnection::Create(boost::asio::io_service& io_service) {
		Storage::Set(istring::literal("toucan"), istring::literal("rasta"));
		return shared_ptr<TCPConnection>(new TCPConnection(io_service));
	}
	
	void TCPConnection::Start() {
		bool found;
		auto key = istring::literal("toucan");
		message_ = Storage::Get(key, &found);
//		if (!found) {
//			Logger(RED) << "404 Not Found: '" << key << "'";
//		} else {
//			Logger(RED) << "'" << key << "' -> '" << message_ << "'";
//		}
		
		boost::asio::async_write(socket_, boost::asio::buffer(message_.std_str()), boost::bind(&TCPConnection::HandleWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	
	void TCPConnection::HandleWrite(const boost::system::error_code& error, size_t bytesTransferred) {
		// nothing
	}
	
	TCPConnection::TCPConnection(boost::asio::io_service& io_service):
		socket_ (io_service)
	{}
}