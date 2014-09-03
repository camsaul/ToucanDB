//
//  Commands.h
//  Money
//
//  Created by Cam Saul on 9/1/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

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
		
		static Command FromInput(string& input);
		static Command Decode	(string&& request);
		
		string Encode() const;
		
		Type			CommandType()	const { return type_; }
		const string&	Key()			const { return key_;  }
		const string&	Val()			const { return val_;  }
		
	private:
		static const map<string, Type> kCommandStrings;
		
		Type type_;
		string key_;
		string val_;
		
		Command() = default;
	};
	
	ostream& operator<<(ostream& os, const Command& c);
}
