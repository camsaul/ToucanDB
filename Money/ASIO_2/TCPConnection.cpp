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
#include "Command.h"
#include "Compression.h"

using namespace toucan_db::logging;

namespace toucan_db {
	shared_ptr<TCPConnection> TCPConnection::Create(boost::asio::io_service& io_service) {
		return shared_ptr<TCPConnection>(new TCPConnection(io_service));
	}
	
	TCPConnection::TCPConnection(boost::asio::io_service& io_service) {
		SetSocket(io_service);
	}
	
	void TCPConnection::Start() {
		ReadAsync([self = shared_from_this()](string&& request){
			try {
				auto c = Command::Decode(std::move(request));
				switch (c.CommandType()) {
					case Command::Type::GET: {
						auto val = Storage::Get(c.Key());
						self->WriteSync(!val.empty() ? val.std_str() : "\0");
					} break;
					case Command::Type::SET: {
						Storage::Set(c.Key(), c.Val());
						self->WriteSync("ok");
					} break;
					case Command::Type::DELETE: {
						Storage::Delete(c.Key());
						self->WriteSync("ok");
					} break;
					default: {
						throw runtime_error("Invalid command.");
					} break;
				}
			} catch (exception& e) {
				string msg = "Error: ";
				msg += e.what();
				self->WriteSync(msg.c_str());
			}
			self->Start();
		});
	}
}