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
	{
		assert(Type() == DataType::LONG_STR);
		
		auto numBlocks = len / kLongStringDataSize;
		if (len % kLongStringDataSize) numBlocks++;
		Tag().len = numBlocks;
		SetPtr(new LongStringData[numBlocks]);
		
		memset((void *)LastBlock(), 0, kLongStringDataSize); // zero out last block
		
		memcpy((void *)Ptr(), str, len);
	}
	
	LongString::~LongString() {
		cout << "~LongString() " << hex << Ptr() << dec << endl;
		delete[] Ptr();
	}
	
	LongString& LongString::operator=(LongString&& rhs) {
		TaggedPtrVal::operator=(static_cast<TaggedPtrVal&&>(rhs));
		assert(Type() == DataType::LONG_STR);
		return *this;
	}
	
	const LongStringData* LongString::LastBlock() const {
		return Ptr() + (Tag().len - 1);
	}
	
	size_t LongString::LastBlockLength() const {
		return strlen(LastBlock()->data);
	}

}