//
//  BasicConnection.cpp
//  Money
//
//  Created by Cam Saul on 8/30/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "BasicConnection.h"
#include "Logging.h"

using namespace toucan_db::logging;

namespace toucan_db {
	void BasicConnection::Disconnect() {
		if (socket_) {
			socket_->close();
			socket_ = nullptr;
		}
	}
	
	void BasicConnection::WriteSync(const char* msg) {
		if (!SocketIsOpen()) return;
		
		strncpy(writeBuffer_.data(), msg, 256);
		
		boost::system::error_code error;
		Socket().write_some(boost::asio::buffer(writeBuffer_), error);
		
		if (error) {
			if (error == boost::asio::error::eof) {
				Disconnect();
				return;
			} else {
				Logger(RED) << "BasicConnection::WriteSync() Error: " << boost::system::system_error(error).what();
			}
		}
	}
	
	void BasicConnection::WriteAsync(const char* msg, AsyncCallback callback) {
		auto shared = shared_from_this();
		if (!shared) return;
		
		if (!SocketIsOpen()) return;
		
		strncpy(writeBuffer_.data(), msg, 256);
		
		asyncWriteCallback_ = callback;
		
		try {
			socket_->async_write_some(boost::asio::buffer(writeBuffer_), boost::bind(&BasicConnection::HandleWriteAsync, shared, boost::asio::placeholders::error()));
		} catch (exception& e) {
			Logger(RED) << "Caught exception in async_write_some: " << e.what();
		}
	}
	
	void BasicConnection::HandleWriteAsync(const boost::system::error_code& error) {
		auto shared = shared_from_this();
		if (!shared) return;
		
		if (error) {
			if (error == boost::asio::error::eof) {
				Disconnect();
				return;
			} else {
				Logger(RED) << "BasicConnection::HandleWriteAsync() Error: " << boost::system::system_error(error).what();
				return;
			}
		}
		
		asyncWriteCallback_(shared);
	}
	
	const char* BasicConnection::Read() {
		if (!SocketIsOpen()) return nullptr;
	
		boost::system::error_code error;
		Socket().read_some(boost::asio::buffer(readBuffer_), error);
		
		if (error) {
			if (error == boost::asio::error::eof) {
				Disconnect();
			} else {
				Logger(RED) << "BasicConnection::Read() Error: " << boost::system::system_error(error).what();
			}
			return nullptr;
		}
		
		return readBuffer_.data();
	}
}