//
//  StrTest2.cpp
//  Money
//
//  Created by Cam Saul on 9/8/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "StrTest.h"
#include "TString.h"
#include "Logging.h"
#include "Timed.h"

using namespace toucan_db;
using namespace toucan_db::logging;

namespace str_test {
	void SetUp(size_t len);
	char* GetCString(size_t trash);
	string& GetString(size_t trash);
	const toucan_db::TString& GetTString(size_t trash);
	
	void Run() {
		for (int strSize = 2; strSize < 1000000; strSize *= 10) {
			Logger(RED) << "---------------------- len = " << strSize << " ----------------------";
			
			SetUp(strSize);
			
			Logger(RED) << "char*";
			Timed1Million([&](size_t i){ assert(strlen(GetCString(i)) == strSize - 1); });
			
			Logger(RED) << "std::string";
			Timed1Million([&](size_t i){ assert(GetString(i).length() == strSize - 1); });
			
			Logger(RED) << "TString";
			Timed1Million([&](size_t i){ assert(GetTString(i).Length() == strSize - 1); });
		}
		
		// strcmp test
		for (int strSize = 2; strSize < 1000000; strSize *= 10) {
			Logger(RED) << "---------------------- compare = " << strSize << " ----------------------";
			
			SetUp(strSize);
			
			Logger(RED) << "char*";
			Timed1Million([&](size_t i){ assert(!strcmp(GetCString(i), GetCString(i/2))); });
			
			Logger(RED) << "std::string";
			Timed1Million([&](size_t i){ assert(GetString(i) == GetString(i/2)); });
			
			Logger(RED) << "TString";
			Timed1Million([&](size_t i){ assert(GetTString(i) == GetTString(i/2)); });
		}
	}
}