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
	istring::literal("toucan_0"),
	istring::literal("toucan_1"),
	istring::literal("toucan_2"),
	istring::literal("toucan_3"),
	istring::literal("toucan_4"),
	istring::literal("toucan_5"),
	istring::literal("toucan_6"),
	istring::literal("toucan_7"),
	istring::literal("toucan_8"),
	istring::literal("toucan_9"),
	istring::literal("toucan_10"),
	istring::literal("toucan_11"),
	istring::literal("toucan_12"),
	istring::literal("toucan_13"),
	istring::literal("toucan_14"),
	istring::literal("toucan_15"),
	istring::literal("toucan_16"),
	istring::literal("toucan_17"),
	istring::literal("toucan_18"),
	istring::literal("toucan_19"),
	istring::literal("toucan_20"),
	istring::literal("toucan_21"),
	istring::literal("toucan_22"),
	istring::literal("toucan_23"),
	istring::literal("toucan_24"),
	istring::literal("toucan_25"),
	istring::literal("toucan_26"),
	istring::literal("toucan_27"),
	istring::literal("toucan_28"),
	istring::literal("toucan_29"),
	istring::literal("toucan_30"),
	istring::literal("toucan_31"),
};

static atomic<int> sCounter;

void toucan_db::SetTheCans() {
	for (auto k : sKeys) {
		Storage::Set(k, "rasta");
	}
}

inline istring GetANewKey() {
	return std::move(sKeys[sCounter++ % 32]);
}

namespace toucan_db {
	shared_ptr<TCPConnection> TCPConnection::Create(boost::asio::io_service& io_service) {
		return shared_ptr<TCPConnection>(new TCPConnection(io_service));
	}
	
	void TCPConnection::Start() {
		bool found;
		message_ = Storage::Get(key_, &found);
		
		boost::asio::async_write(socket_, boost::asio::buffer(message_.std_str()), boost::bind(&TCPConnection::HandleWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	
	void TCPConnection::HandleWrite(const boost::system::error_code& error, size_t bytesTransferred) {
		// nothing
	}
	
	TCPConnection::TCPConnection(boost::asio::io_service& io_service):
		socket_ (io_service)
	{
		key_ = GetANewKey();
		Storage::Set(key_, istring::literal("rasta"));
	}
}