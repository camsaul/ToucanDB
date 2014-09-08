//
//  TString.h
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include "ShortString.h"
#include "LongString.h"

namespace toucan_db {
	class TString {
	public:
		TString() = default;
		TString(char* str);
		
		size_t Length() const {
			return IsShortString() ? data_.shortStr.Length() : data_.longStr.Length();
		}
		
		friend ostream& operator<<(ostream& os, const TString& str);
		
	private:
		__attribute((noinline)) bool IsShortString() const {
			return data_.type.value == DataType::SHORT_STR;
		}
				
		union Data {
			TypeInfo type;
			size_t raw;
			LongString longStr;
			ShortString shortStr;
			
			Data():
				raw(static_cast<size_t>(DataType::SHORT_STR))
			{}
			
		} data_;
	};
	
	static_assert(sizeof(TString) == 8, "TString should be 8 bytes!");
}