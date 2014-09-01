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

void toucan_db::SetTheCans() {
	for (auto k : toucan_db::sKeys) {
		Storage::Set(k, "rasta");
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
		ReadAsync([self = shared_from_this()](const char* request){
			if (!request) return;
			self->WriteSync(Storage::Get(request));
			self->Loop();
		});
	}
	
	void TCPConnection::Loop() {
		const char* request = nullptr;
		while ((request = Read())) {
			WriteSync(Storage::Get(request));
		}
	}
//	
//	void TCPConnection::HandleRequest(const char* request) {
//		bool found = false;
//		auto value = Storage::Get(request, &found);
//		assert(found);
//		
//		WriteAsync(value, [self = shared_from_this()](){
//			self->Start();
//		});
//	}
}