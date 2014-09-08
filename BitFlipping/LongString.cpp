//
//  LongString.cpp
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "LongString.h"

namespace toucan_db {
	ostream& operator<<(ostream& os, const LongString& str) {
		os << str.Ptr();
		return os;
	}
	
//	LongString::LongString(const char* str):
//		LongString(str, (strlen(str) / 8) + 1)
//	{}
	
	LongString::LongString(const char* str, size_t len)
//		TaggedPtrVal((len / 8) + 1, str)
	{
		assert(Type() == DataType::LONG_STR);
		
		auto numBlocks = len / 8;
		if (len % 8) numBlocks++;
		Tag().len = numBlocks;
		SetPtr(new LongStringData[numBlocks]);
		
		memset((void *)LastBlock(), 0, 8); // zero out last block
		
		memcpy((void *)Ptr(), str, len);
	}
	
	const LongStringData* LongString::LastBlock() const {
		return Ptr() + (Tag().len - 1);
	}
	
	size_t LongString::LastBlockLength() const {
		return strlen(LastBlock()->data);
	}

}