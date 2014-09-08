//
//  ShortString.cpp
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "ShortString.h"

namespace toucan_db {
	ShortString::ShortString(const char* str) {
		assert((data_.raw & 0b111) == static_cast<size_t>(DataType::SHORT_STR));
		assert(data_.d.type == DataType::SHORT_STR); // should be set by default constructor
		int i;
		for (i = 0; i < 7; i++) {
			char c = str[i];
			if (c == 0) {
				break;
			}
			data_.d.str[i] = c;
		}
		data_.d.size = i;
		assert((data_.raw & 0b111) == static_cast<size_t>(DataType::SHORT_STR));
	}
	
	ostream& operator<<(ostream& os, const ShortString& str) {
		os << str.data_.d.str;
		return os;
	}
}