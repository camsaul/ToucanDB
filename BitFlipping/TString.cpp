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
			data_.shortStr = ShortString { str };
			assert(Type() == DataType::SHORT_STR);
		} else {
			cout << "(long str)" << endl;
			data_.longStr = LongString{ str, len };
			assert(Type() == DataType::LONG_STR);
		}
	}
	
	TString::TString(TString&& rhs):
		data_(std::move(rhs.data_))
	{}
	
	TString& TString::operator==(TString&& rhs) {
		if (this != &rhs) {
			data_.raw = rhs.data_.raw;
			rhs.data_.raw = 0;
		}
		return *this;
	}
	
	DataType TString::Type() const {
		return static_cast<DataType>(data_.raw & 0b111);
	}
	
	TString::Data::Data():
		raw(static_cast<size_t>(DataType::SHORT_STR))
	{}
	
	TString::Data::Data(Data&& rhs):
		raw(rhs.raw)
	{
		if (this != &rhs) {
			rhs.raw = 0;
		}
	}
	
	TString::Data::~Data() {
		if (raw & 1) {
			delete &longStr;
		}
	}

	ostream& operator<<(ostream& os, const TString& str) {
		if (str.IsLongString()) {
			os << str.data_.longStr;
		} else {
			os << str.data_.shortStr;
		}
		return os;
	}
}