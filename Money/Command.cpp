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
	
	ostream& operator<<(ostream& os, const Command& c) {
		os << static_cast<char>(c.CommandType()) << ' ' << c.Key() << ' ' << (c.Val() ? c.Val() : "");
		return os;
	}
}