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
		
		auto numBlocks = len / 8;
		if (len % 8) numBlocks++;
		Tag().len = numBlocks;
		SetPtr(new LongStringData[numBlocks]);
		cout << "LongString() -> " << hex << Ptr() << dec << endl;
		
		memset((void *)LastBlock(), 0, 8); // zero out last block
		
		memcpy((void *)Ptr(), str, len);
	}
	
	LongString::~LongString() {
		cout << "~LongString() " << hex << Ptr() << dec << endl;
		delete[] Ptr();
	}
	
	LongString& LongString::operator=(LongString&& rhs) {
		if (this != &rhs) {
			Data().raw = rhs.Data().raw;
			rhs.Data().raw = 0;
		}
		return *this;
	}
	
	const LongStringData* LongString::LastBlock() const {
		return Ptr() + (Tag().len - 1);
	}
	
	size_t LongString::LastBlockLength() const {
		return strlen(LastBlock()->data);
	}

}