//
//  TCPConnection.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

namespace toucan_db {
	using boost::asio::ip::tcp;
	
	void SetTheCans();
	
	class TCPConnection : public enable_shared_from_this<TCPConnection> {
	public:
		static shared_ptr<TCPConnection> Create(boost::asio::io_service& io_service);
		
		tcp::socket& Socket() { return socket_; }
		
		void Start();
		void HandleWrite(const boost::system::error_code& error, size_t bytesTransferred);
		
	private:
		TCPConnection(boost::asio::io_service& io_service);
		
		tcp::socket socket_;
		istring message_;
		istring key_;
	};
}