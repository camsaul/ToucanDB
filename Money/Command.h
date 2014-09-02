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
		static char* EncodeInput(char* raw);
		
		static Command Decode(char* raw);
		
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
