//
//  Client.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
	using boost::asio::ip::tcp;
	class Client : private boost::noncopyable {
	public:
		Client(string host = "172.20.10.3", int16_t port = 1337);
		Client(Client&&);
		
		void Connect(size_t iterations);
		
		
	private:
		string  host_;
		int16_t port_;
		
		boost::array<char, 256> buffer_;
		
		unique_ptr<boost::asio::io_service> ioService_;
		unique_ptr<tcp::resolver>			resolver_;
		tcp::resolver::query				query_;
		tcp::resolver::iterator				endpoint_iterator_;
		tcp::socket							socket_;
	};
}