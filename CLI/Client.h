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
		
		static shared_ptr<Client> Create(string host = "172.20.10.3", int16_t port = 1337);
		
		void Connect();
		void Start();
		void SendRequest();
		void ReadResponse();
		
	private:
		Client(string host, int16_t port);
		string  host_;
		int16_t port_;
	};
}