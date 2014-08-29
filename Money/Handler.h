//
//  Handler.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace crow {
	class request;
	class response;
}

class Handler {
public:
	Handler();
	
	~Handler() {
		cout << "~Handler()" << endl;
	}
	
	static void Handle(const crow::request& req, crow::response& res);
	
	/// TODO - How to get key?
	static void HandleRequest(const crow::request& req, crow::response& res, const istring& key);
	
private:
	uint16_t port_ = 1337;
	uint16_t concurrency_;
};