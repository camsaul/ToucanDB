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

		strcpy(writeBuffer_.data(), msg);
		Socket().write_some(boost::asio::buffer(writeBuffer_), error_);
		if (!error_) return;
		
		if (error_ == boost::asio::error::eof)	Disconnect();
		else									Logger(RED) << "BasicConnection::WriteSync() Error: " << boost::system::system_error(error_).what();
	}
	
	void BasicConnection::WriteAsync(const char* msg, AsyncWriteCallback callback) {
		asyncWriteCallback_ = callback;
		WriteAsync(msg);
	}
	
	void BasicConnection::WriteAsync(const char* msg) {
		if (!SocketIsOpen()) return;
		strcpy(writeBuffer_.data(), msg);
		Socket().async_write_some(boost::asio::buffer(writeBuffer_), boost::bind(&BasicConnection::HandleWriteAsync, this, boost::asio::placeholders::error()));
	}
	
	void BasicConnection::HandleWriteAsync(const boost::system::error_code& error) {
		if (error) {
			if (error == boost::asio::error::eof)	Disconnect();
			else									Logger(RED) << "BasicConnection::HandleWriteAsync() Error: " << boost::system::system_error(error).what();
			return;
		}
		
		asyncWriteCallback_();
	}
	
	const char* BasicConnection::Read() {
		if (!SocketIsOpen()) return nullptr;

		Socket().read_some(boost::asio::buffer(readBuffer_), error_);
		if (!error_) return readBuffer_.data();
		
		if (error_ == boost::asio::error::eof)	Disconnect();
		else									Logger(RED) << "BasicConnection::Read() Error: " << boost::system::system_error(error_).what();
		return nullptr;
	}
	
	void BasicConnection::ReadAsync(AsyncReadCallback callback) {
		assert(callback);
		if (!SocketIsOpen()) return;
		
		asyncReadCallback_ = callback;
		
		Socket().async_read_some(boost::asio::buffer(readBuffer_), boost::bind(&BasicConnection::HandleReadAsync, this, boost::asio::placeholders::error()));
	}
	
	void BasicConnection::HandleReadAsync(const boost::system::error_code& error) {
		if (error) {
			if (error == boost::asio::error::eof) {
				Disconnect();
			} else {
				Logger(RED) << "BasicConnection::HandleReadAsync() Error: " << boost::system::system_error(error).what();
			}
			return;
		}
		
		asyncReadCallback_(readBuffer_.data());
	}
}