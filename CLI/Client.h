//
//  Client.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include "BasicConnection.h"

namespace toucan_db {
	class Client : public BasicConnection {
	public:
		static const size_t kNumIterations;
		static atomic<int> sRequestsCount;
		
		static shared_ptr<Client> Create(string host = "10.10.30.236", int16_t port = 1337);
		
		string Request(const string& msg);
		
	private:
		Client(string host, int16_t port);
		string  host_;
		int16_t port_;
	};
}