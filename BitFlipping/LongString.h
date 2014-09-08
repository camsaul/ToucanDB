//
//  LongString.h
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include "DataType.h"

namespace toucan_db {
	struct LongStringUpperTag {
		size_t len : 16;
		
		LongStringUpperTag(size_t l):
			len(l)
		{}
	} __attribute__((packed));
	static_assert(sizeof(LongStringUpperTag) == 2, "LongStringUpperTag should be 2 bytes!");
	
	class LongString : public TaggedPtrVal<DataType::LONG_STR, char, LongStringUpperTag> {
	public:
		LongString(char* str):
			TaggedPtrVal(strlen(str), str)
		{}
		
		size_t Length() const {
			return Tag().len;
		}
		
		friend ostream& operator<<(ostream& os, const LongString& str);
	};
	
	static_assert(sizeof(LongString) == 8, "LongString should be 8 bytes!");
}