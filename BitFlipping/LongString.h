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
	
	struct LongStringData {
		char data[8];
	};
	static_assert(sizeof(LongStringData) == 8, "LongStringData should be 8 bytes!");
	
	class LongString : public TaggedPtrVal<DataType::LONG_STR, LongStringData, LongStringUpperTag> {
	public:
//		LongString(const char* str);
		LongString(const char* str, size_t len);
		
		size_t Length() const { return ((NumBlocks() - 1) * 8) + LastBlockLength(); }
		
		bool operator==(const LongString& rhs) const {
			const bool equalLengths = Tag().len == rhs.Tag().len;
			return equalLengths ? (memcmp(Ptr(), rhs.Ptr(), Tag().len) == 0) : false;
		}
		
		friend ostream& operator<<(ostream& os, const LongString& str);
		
	private:
		size_t NumBlocks() const { return Tag().len; }
		const LongStringData* LastBlock() const;
		size_t LastBlockLength() const;
	};
	
	static_assert(sizeof(LongString) == 8, "LongString should be 8 bytes!");
}