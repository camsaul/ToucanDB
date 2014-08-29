//
//  Handler.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "http_server.h"
#include "http_request.h"
#include "http_response.h"

#include "Handler.h"
#include "Storage.h"

Handler::Handler():

	concurrency_(thread::hardware_concurrency())
{
	cout << "STARTING ON PORT " << port_  << " WITH " << concurrency_ << " THREADS." << endl;
	crow::Server server { this, port_, concurrency_ };
	server.run();
}

void Handler::Handle(const crow::request& req, crow::response& res) {
	cout << "Handler::Handle()" << endl;
	HandleRequest(req, res, istring::literal("toucan")); // TODO
}

void Handler::HandleRequest(const crow::request& req, crow::response& res, const istring& key) {
	cout << "Handler::HandleRequest()" << endl;
	switch (req.method) {
		case crow::HTTPMethod::GET: {
			cout << key << endl;
			bool found;
			res.body_ = Storage::Get(key, &found);
			if (!found) res.code = 404;
			res.end();
			return;
		} break;
		case crow::HTTPMethod::PUT:
		case crow::HTTPMethod::POST: {
			cout << key << " -> " << req.body << endl;
			Storage::Set(key, req.body);
			res.code = 201;
			res.end(istring::literal("ok"));
			return;
		} break;
		case crow::HTTPMethod::DELETE: {
			Storage::Delete(key);
			res.code = 204;
		} break;
		default: {
			res.code = 500;
		} break;
	}
	
	res.end();
	return;
}