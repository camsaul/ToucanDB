//
//  LongString.h
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include "TaggedPtrValue.h"

namespace toucan_db {
	struct LongStringUpperTag {
		size_t len : 16;
		
		LongStringUpperTag(size_t l):
			len(l)
		{}
	} __attribute__((packed));
	static_assert(sizeof(LongStringUpperTag) == 2, "LongStringUpperTag should be 2 bytes!");
	

	/// TODO - Rewrite the operator== code to use SSE
	static const size_t kLongStringDataSize = 16; // 128-bit
	struct LongStringData {
		char data[kLongStringDataSize];
	};
	static_assert(sizeof(LongStringData) == kLongStringDataSize, "LongStringData is wrong number of bytes!");
	
	class LongString : public TaggedPtrVal<DataType::LONG_STR, LongStringData, LongStringUpperTag> {
	public:
		LongString(const char* str, size_t len);
		~LongString();
		
		LongString& operator=(LongString&& rhs);
		
		size_t Length() const { return ((NumBlocks() - 1) * kLongStringDataSize) + LastBlockLength(); }
		
		bool operator==(const LongString& rhs) const {
			const bool equalLengths = Tag().len == rhs.Tag().len;
			return equalLengths ? (memcmp(Ptr(), rhs.Ptr(), Tag().len * kLongStringDataSize) == 0) : false;
		}
		
		friend ostream& operator<<(ostream& os, const LongString& str);
		
	private:
		size_t NumBlocks() const { return Tag().len; }
		const LongStringData* LastBlock() const;
		size_t LastBlockLength() const;
	};
	
	static_assert(sizeof(LongString) == 8, "LongString should be 8 bytes!");
}