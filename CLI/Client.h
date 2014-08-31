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
	class Client : public enable_shared_from_this<Client>, private boost::noncopyable {
	public:
		static const size_t kNumIterations;
		static atomic<int> sRequestsCount;
		static atomic<int> sReadsCount;
		
		Client(string host = "172.20.10.3", int16_t port = 1337);
		Client(Client&&);
		
//		~Client() {
//			cout << "~Client()" << endl;
//		}
		
		void Connect(); ///< Connect repeatedly decrementing numIterations_ until it is 0
//		void Connect(int iterations); ///< Set numIterations_, then call regular Connect()
		void HandleConnect(const boost::system::error_code& error);
		
		void Read();
		void HandleRead(const boost::system::error_code&);
		
	private:
		string  host_;
		int16_t port_;
		
		boost::array<char, 256> buffer_;
//		int numIterations_ = 1;
		
		unique_ptr<boost::asio::io_service> ioService_;
		unique_ptr<tcp::resolver>			resolver_;
		tcp::resolver::query				query_;
		tcp::resolver::iterator				endpoint_iterator_;
		tcp::socket							socket_;
	};
}