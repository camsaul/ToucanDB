//
//  Command.cpp
//  Money
//
//  Created by Cam Saul on 9/1/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Command.h"
#include "Compression.h"
#include "Logging.h"

using namespace toucan_db::logging;

namespace toucan_db {
	const map<string, Command::Type> Command::kCommandStrings {
		{"set",		Type::SET},
		{"get",		Type::GET},
		{"delete",	Type::DELETE},
	};
	
	Command Command::FromInput(string& input) {
		if (input.empty()) {
			throw runtime_error("No command specified");
		}
 		Command c;
		
		char* raw = const_cast<char*>(input.data());
		
		//********** type **********//
		string type { strtok(raw, " ") };
		auto itr = kCommandStrings.find(type);
		if (itr == kCommandStrings.end()) {
			throw runtime_error("Invalid command: '" + type + "'");
		}
		c.type_ = itr->second;
		
		//********** key **********//
		char* key = strtok(nullptr, " ");
		c.key_ = key;
		if (c.key_.empty()) {
			throw runtime_error("Usage: " + type + " [key]"); // TODO: Needs to add "[value]" for SET
		}
		
		//********** val **********//
		char* valStart = key + c.key_.length() + 1;
		auto strEnd = &*input.end() + 1; // final '\0'
		ssize_t valLen = strEnd - valStart;
		assert(valLen >= 0);
		assert(valLen < input.length());
		if (valLen) {
			c.val_ = { valStart, static_cast<size_t>(valLen - 1)}; // don't include the null byte as part of the string
		}
		
		return c;
	}
	
	Command Command::Decode(string&& request) {
		if (request.empty()) {
			throw runtime_error { "Invalid request: " + request };
		}
		Command c;
		
		char* raw = const_cast<char*>(request.data());
		
		//********** type **********//
		c.type_ = static_cast<Type>(raw[0]);
		
		//********** key **********//
		raw++;
		char* key = strtok(raw, " "); // null-terminate after end of key
		c.key_ = key;
		if (c.key_.empty()) {
			throw runtime_error { "Key cannot be empty!" };
		}
		
		//********** val **********//
		char* valStart = key + c.key_.length() + 1;
		auto strEnd = &*request.end() + 1; // final '\0'
		ssize_t valLen = strEnd - valStart;
		assert(valLen >= 0);
		assert(valLen < request.length());
		assert(valStart > raw);
		assert(valStart <= strEnd);
		if (valLen) {
			c.val_ = { valStart, static_cast<size_t>(valLen - 1) }; // don't include the null byte as part of the string
		}
		
		return c;
	}
	
	string Command::Encode() const {
		string output;
		output += static_cast<char>(type_);
		output += key_;
		if (!val_.empty()) {
			output += ' ';
			output += Compress(val_);
		}
		return output;
	}
	
	ostream& operator<<(ostream& os, const Command& c) {
		os << static_cast<char>(c.CommandType()) << ' ' << c.Key() << ' ' << c.Val();
		return os;
	}
}