//
//  TCPConnection.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "TCPConnection.h"
#include "Storage.h"
#include "Logging.h"

using namespace toucan_db::logging;

static vector<istring> sKeys {
	istring::literal("toucan"),
};

void toucan_db::SetTheCans() {
	for (auto k : sKeys) {
		Storage::Set(k.c_str(), "rasta");
	}
}

namespace toucan_db {
	shared_ptr<TCPConnection> TCPConnection::Create(boost::asio::io_service& io_service) {
		return shared_ptr<TCPConnection>(new TCPConnection(io_service));
	}
	
	TCPConnection::TCPConnection(boost::asio::io_service& io_service) {
		SetSocket(io_service);
	}
	
	void TCPConnection::Start() {
		WriteAsync("OK.", [](shared_ptr<BasicConnection> self_){
			auto self = dynamic_cast<TCPConnection*>(self_.get());
			if (!self) return;
			
			auto request = self->Read();
			if (!request) return;
			self->HandleRequest(request);
		});
	}
	
	void TCPConnection::HandleRequest(const char* request) {
		bool found = false;
		auto value = Storage::Get(request, &found);
		
		WriteAsync(value, [](shared_ptr<BasicConnection> self_){
			auto self = dynamic_cast<TCPConnection*>(self_.get());
			if (!self) return;
			self->Start();
		});
	}
}