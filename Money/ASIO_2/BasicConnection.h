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
	
	class BasicConnection : public enable_shared_from_this<BasicConnection>, private boost::noncopyable {
	public:
		using AsyncCallback = function<void(shared_ptr<BasicConnection>)>;
		
		tcp::socket& Socket() { return *socket_; }
		bool SocketIsOpen() const { return socket_ && socket_->is_open(); }
		
		void Disconnect();
	protected:
		virtual ~BasicConnection() = default;
		
		void SetSocket(boost::asio::io_service& io_service) { socket_ = make_unique<tcp::socket>(io_service); }
				
		void WriteSync(const char* msg);
		void WriteAsync(const char* msg, AsyncCallback callback);
		
		const char* Read();
		
		virtual void HandleWriteAsync(const boost::system::error_code& error);
		
	private:
		unique_ptr<tcp::socket> socket_ = nullptr;
		boost::array<char, 256> readBuffer_;
		boost::array<char, 256> writeBuffer_;
		AsyncCallback asyncWriteCallback_;
		
		
	};
}