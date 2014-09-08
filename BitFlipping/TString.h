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
		TString(const char* str);
		
		size_t Length() const { return IsLongString() ? data_.longStr.Length() : data_.shortStr.Length(); }
		
		friend ostream& operator<<(ostream& os, const TString& str);
		
	private:
		DataType Type() const;
		bool IsLongString() const { return data_.raw & 1; }
				
		union Data {
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