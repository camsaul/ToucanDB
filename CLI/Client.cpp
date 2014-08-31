//
//  Client.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Client.h"
#include "Logging.h"

using boost::asio::ip::tcp;
using namespace toucan_db::logging;

static const char * const sKeys[] {
	"toucan_0",
	"toucan_1",
	"toucan_2",
	"toucan_3",
	"toucan_4",
	"toucan_5",
	"toucan_6",
	"toucan_7",
	"toucan_8",
	"toucan_9",
	"toucan_10",
	"toucan_11",
	"toucan_12",
	"toucan_13",
	"toucan_14",
	"toucan_15",
	"toucan_16",
	"toucan_17",
	"toucan_18",
	"toucan_19",
	"toucan_20",
	"toucan_21",
	"toucan_22",
	"toucan_23",
	"toucan_24",
	"toucan_25",
	"toucan_26",
	"toucan_27",
	"toucan_28",
	"toucan_29",
	"toucan_30",
	"toucan_31",
};
inline const char * GetANewKey() {
	static int sCounter = 0;
	return sKeys[++sCounter % 32];
}

namespace toucan_db {
	atomic<int> Client::sRequestsCount { 0 };
	
	shared_ptr<Client> Client::Create(string host, int16_t port) {
		return shared_ptr<Client>(new Client(host, port));
	}
	
	Client::Client(string host, int16_t port):
		host_				(host),
		port_				(port)
	{}
	
	void Client::Connect() {
		boost::asio::io_service ioService;
		SetSocket(ioService);
		tcp::resolver			resolver			{ ioService };
		tcp::resolver::query	query				{ host_, lexical_cast<string>(port_) };
		auto					endpointIterator	= resolver.resolve(query);
		auto					socket				= make_unique<tcp::socket>(ioService);
		boost::asio::connect(Socket(), endpointIterator);
		
		Start();
	}
	
	void Client::Start() {
		while (++sRequestsCount < kNumIterations) {
			WriteSync(GetANewKey());
			auto response = Read();
		}
		Disconnect();
	}
//	
//	void Client::SendRequest() {
//		WriteSync(GetANewKey());
//		ReadResponse();
//	}
//	
//	void Client::ReadResponse() {
//		auto response = Read();
//		sRequestsCount++;
//	}
}