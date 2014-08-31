//
//  Storage.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Storage.h"

namespace toucan_db {
	Storage::StorageT Storage::sStorage {};
	
	atomic<int> Storage::sNumGets { 0 };
	
	istring Storage::Get(istring key, bool* found) {
		sNumGets++;
		StorageT::const_accessor a;
		return std::move((*found = sStorage.find(a, key)) ? a->second : istring::literal(""));
	}
	
	void Storage::Set(istring key, istring val) {
		sStorage.insert({key, val});
	}
	
	void Storage::Delete(istring key) {
		sStorage.erase(key);
	}
}



