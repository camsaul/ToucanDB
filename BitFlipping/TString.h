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
		
		TString(const TString&) = delete;
		TString(TString&& rhs) = default;
		
		TString& operator=(const TString&) = delete;
		TString& operator=(TString&& rhs);
		
		__attribute__((always_inline)) size_t Length() const { return IsLongString() ? data_.longStr.Length() : data_.shortStr.Length(); }
		
		bool operator==(const TString& rhs) const {
			if (IsLongString()) {
				if (!rhs.IsLongString()) return false;
				
				return data_.longStr == rhs.data_.longStr;
			}
			return data_.shortStr == data_.shortStr;
		}
		
		bool IsLongString() const { return data_.raw & 1; }
		
		friend ostream& operator<<(ostream& os, const TString& str);
		
	private:
		DataType Type() const;
				
		union Data {
			size_t raw;
			LongString longStr;
			ShortString shortStr;
			
			Data();
			Data(const Data&) = delete;
			Data(Data&& rhs);
			
			~Data();
			
		} data_;
	};
	
	static_assert(sizeof(TString) == 8, "TString should be 8 bytes!");
}