//
//  TString.cpp
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "TString.h"

namespace toucan_db {
	TString::TString(char* str) {
		auto len = str ? strlen(str) : 0;
		if (len <= 7) {
			cout << "(short str)" << endl;
			data_.shortStr = str;
			assert(data_.type.value == DataType::SHORT_STR);
		} else {
			cout << "(long str)" << endl;
			data_.longStr = { str, len };
			assert(data_.type.value == DataType::LONG_STR);
		}
	}

	ostream& operator<<(ostream& os, const TString& str) {
		if (str.IsShortString()) {
			os << str.data_.shortStr;
		} else {
			os << str.data_.longStr;
		}
		return os;
	}
}