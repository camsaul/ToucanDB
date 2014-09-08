//
//  StrTest.h
//  Money
//
//  Created by Cam Saul on 9/8/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
	class TString;
}

void SetUp(size_t len);
char* GetCString(size_t trash);
string* GetString(size_t trash);
toucan_db::TString* GetTString(size_t trash);