//
//  StrTest.cpp
//  Money
//
//  Created by Cam Saul on 9/8/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "StrTest.h"
#include "TString.h"

using namespace toucan_db;

namespace str_test {
	static char* sCString = nullptr;
	static string sString;
	static unique_ptr<TString> sTString;

	void SetUp(size_t len) {
		free(sCString);
		sCString = new char[len];
		auto range = 'z' - 'a'; // fill with a-z over and over
		for (int i = 0; i < len - 1; i++) {
			sCString[i] = (i % range) + 'a';
		}
		sCString[len - 1] = 0;
		
		sString = sCString;
		sTString = make_unique<TString>(sCString);
	}

	char* GetCString(size_t trash) {
		return sCString;
	}

	string& GetString(size_t trash) {
		return sString;
	}

	const TString& GetTString(size_t trash) {
		return *sTString;
	}
}