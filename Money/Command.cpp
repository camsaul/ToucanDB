//
//  Command.cpp
//  Money
//
//  Created by Cam Saul on 9/1/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Command.h"

namespace toucan_db {
	const map<string, Command::Type> Command::kCommandStrings {
		{"set",		Type::SET},
		{"get",		Type::GET},
		{"delete",	Type::DELETE},
	};
	
	char* Command::EncodeInput(char* raw) {
		if (!raw) return nullptr;
		
		string type { strtok(raw, " ") };
		
		auto itr = kCommandStrings.find(type);
		if (itr == kCommandStrings.end()) {
			throw runtime_error("Invalid command: '" + type + "'");
		}
		auto typeEnum = itr->second;
		
		auto key = strtok(nullptr, " ");
		if (!key) {
			throw runtime_error("Usage: " + type + " [key]"); // TODO: Needs to add "[value]" for SET
		}
		auto keyLength = strlen(key);
		if (!keyLength) {
			throw runtime_error("Usage: " + type + " [key]"); // TODO: Needs to add "[value]" for SET
		}
		
		raw = key - kKeyOffset;
		raw[0] = (char)typeEnum;
		
		memcpy(raw + 1, &keyLength, sizeof(KeyLengthT));
		*(key + keyLength) = ' '; // don't null-terminate key
		return raw;
	}
	
	Command Command::Decode(char* raw) {
		Command c;
		c.type_ = static_cast<Type>(raw[0]);
		
		KeyLengthT keylen;
		memcpy(&keylen, raw + 1, sizeof(keylen));
		
		if (!keylen) {
			throw runtime_error { "Key length cannot be 0." };
		}
		
		c.key_ = raw + kKeyOffset;
		char* keyEnd = c.key_ + keylen;
		if (*keyEnd) {
			*keyEnd = '\0';
			c.val_ = keyEnd + 1;
		}
		
		return std::move(c);
	}
	
	ostream& operator<<(ostream& os, const Command& c) {
		os << static_cast<char>(c.CommandType()) << ' ' << c.Key() << ' ' << (c.Val() ? c.Val() : "");
		return os;
	}
}