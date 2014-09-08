//
//  TString.cpp
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "TString.h"

namespace toucan_db {
	TString::TString(const char* str) {
		auto len = str ? strlen(str) : 0;
		if (len <= 7) {
			cout << "(short str)" << endl;
			data_.shortStr = str;
			assert(Type() == DataType::SHORT_STR);
		} else {
			cout << "(long str)" << endl;
			data_.longStr = { str, len };
			assert(Type() == DataType::LONG_STR);
		}
	}
	
//	size_t TString::Length() const {
//		return IsLongString() ? data_.longStr.Tag().len : data_.shortStr.data_.d.size;
//		return IsLongString() ? data_.longStr.Length() : data_.shortStr.Length();
//	}
	
	DataType TString::Type() const {
		return static_cast<DataType>(data_.raw & 0b111);
	}
	
//	bool TString::IsLongString() const {
//		return data_.raw & 1;
//	}

	ostream& operator<<(ostream& os, const TString& str) {
		if (str.IsLongString()) {
			os << str.data_.longStr;
		} else {
			os << str.data_.shortStr;
		}
		return os;
	}
}