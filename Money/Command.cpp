//
//  Command.cpp
//  Money
//
//  Created by Cam Saul on 9/1/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Command.h"
#include "Logging.h"

using namespace toucan_db::logging;

namespace toucan_db {
	const map<string, Command::Type> Command::kCommandStrings {
		{"set",		Type::SET},
		{"get",		Type::GET},
		{"delete",	Type::DELETE},
	};
	
	string Command::EncodeInput(char* raw) {
		if (!raw) return {};
		
		string output;
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
		KeyLengthT keyLength = strlen(key);
		if (!keyLength) {
			throw runtime_error("Usage: " + type + " [key]"); // TODO: Needs to add "[value]" for SET
		}
		
		output += static_cast<char>(typeEnum);
		output += keyLength;
		output += key;
		output += ' ';
		
		auto val = strtok(nullptr, "");
		
		if (val) {
			auto valLen = strlen(val);
			string compressed;
			google::protobuf::io::StringOutputStream o(&compressed);
			auto co = unique_ptr<zerocc::AbstractCompressedOutputStream>(get_compressed_output_stream(&o, zerocc::LZ4, 1));
			{
				google::protobuf::io::CodedOutputStream c(co.get());
				c.WriteVarint32(valLen);
				c.WriteString(val);
			}
			co->Flush();
			auto compressionPercentage = round(((100.0 * valLen) / compressed.length()) - 100.0);
			Logger(ORANGE) << "[compressed " << compressionPercentage << "%]";
			output += compressed;
		}
		
		return output;
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
	
	const char* Command::Val() const {
		return val_;
	}
	
	ostream& operator<<(ostream& os, const Command& c) {
		os << static_cast<char>(c.CommandType()) << ' ' << c.Key() << ' ' << (c.Val() ? c.Val() : "");
		return os;
	}
}