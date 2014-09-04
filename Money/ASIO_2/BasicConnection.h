//
//  BasicConnection.h
//  Money
//
//  Created by Cam Saul on 8/30/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
	using boost::asio::ip::tcp;
	
	class BasicConnection : private boost::noncopyable {
	public:
		using AsyncWriteCallback = function<void()>;
		using AsyncReadCallback = function<void(string&& data)>;
		
		tcp::socket& Socket()				{ return *socket_; }
		inline bool SocketIsOpen() const	{ return socket_ && socket_->is_open(); }
		
		void Disconnect();
	protected:
		void SetSocket(boost::asio::io_service& io_service) { socket_ = make_unique<tcp::socket>(io_service); }
		void SetAsyncWriteCallback(AsyncWriteCallback callback) { asyncWriteCallback_ = callback; assert(callback); }
				
		void WriteSync(const string& msg);
		void WriteAsync(const string& msg, AsyncWriteCallback callback);
		
		void WriteAsync(const string& msg); // assuming you've already set the callback
		
		string Read();
		void ReadAsync(AsyncReadCallback callback);
	private:
		unique_ptr<tcp::socket> socket_ = nullptr;

//		array<char, 256> writeBuffer_;
		array<char, 256> readBuffer_;
		
		AsyncWriteCallback asyncWriteCallback_;
		AsyncReadCallback asyncReadCallback_;
		
		boost::system::error_code error_;
		
		void HandleWriteAsync(const boost::system::error_code& error);
		void HandleReadAsync(const boost::system::error_code& error);
	};
}