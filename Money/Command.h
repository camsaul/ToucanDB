//
//  Commands.h
//  Money
//
//  Created by Cam Saul on 9/1/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include <cstring>

#include <boost/iostreams/filter/bzip2.hpp>

namespace toucan_db {
	/// A ToucanDB command for transmission from client -> server
	/// [1-byte Command::Type][2-byte key length][key]space[value]
	struct Command {
	public:
		using KeyLengthT = uint8_t;
		static const size_t kKeyOffset = 1 + sizeof(KeyLengthT);
		
		enum class Type : char {
			SET		= 's',
			GET		= 'g',
			DELETE	= 'd'
		};
		
		/// in-situ parsing of command from user input
		static char* EncodeInput(char* raw) {
			string type { strtok(raw, " ") };
			
			auto itr = kCommandStrings.find(type);
			if (itr == kCommandStrings.end()) {
				throw runtime_error("Invalid command: '" + type + "'");
			}
			auto typeEnum = itr->second;
			
			auto key = strtok(nullptr, " ");
			auto keyLength = strlen(key);
			if (!key || !keyLength) {
				throw runtime_error("Usage: " + type + " [key]"); // TODO: Needs to add "[value]" for SET
			}
			
			raw = key - kKeyOffset;
			raw[0] = (char)typeEnum;

			memcpy(raw + 1, &keyLength, sizeof(KeyLengthT));
			*(key + keyLength) = ' '; // don't null-terminate key
			
//			auto val = strtok(raw, " ");
//			boost::iostreams::bzip2_compressor compressor;
//			compressor.write(<#Sink &snk#>, <#const char_type *s#>, <#std::streamsize n#>)
			
			return raw;
		}
		
		static Command Decode(char* raw) {
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
		
		Type		CommandType()	const { return type_; }
		const char* Key()			const { return key_; }
		const char* Val()			const { return val_; }
		
	private:
		static const map<string, Type> kCommandStrings;
		
		Type type_;
		char* key_;
		char* val_ = nullptr;
		
		Command() = default;
	};
	
	ostream& operator<<(ostream& os, const Command& c);
}
