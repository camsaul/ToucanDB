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
	
	class LongString : public TaggedPtrVal<DataType::LONG_STR, const char, LongStringUpperTag> {
	public:
		LongString(const char* str):
			LongString(str, strlen(str))
		{}
		
		LongString(const char* str, size_t len):
			TaggedPtrVal(len, str)
		{
			assert(Type() == DataType::LONG_STR);
			assert(Ptr() == str);
		}
		
		size_t Length() const { return Tag().len; }
		
		bool operator==(const LongString& rhs) const {
			return strcmp(Ptr(), rhs.Ptr()) == 0;
		}
		
		friend ostream& operator<<(ostream& os, const LongString& str);
	};
	
	static_assert(sizeof(LongString) == 8, "LongString should be 8 bytes!");
}