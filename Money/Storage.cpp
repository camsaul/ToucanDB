//
//  Storage.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Storage.h"

struct istring_hash {
	inline static size_t hash(const istring& x ) {
		return x.hash();
    }
	
    inline static bool equal(const istring& x, const istring& y ) {
        return x == y;
    }
};

using StorageT = tbb::concurrent_hash_map<istring, istring, istring_hash>;

static StorageT s_storage {};

istring Storage::Get(istring key, bool* found) {
	StorageT::const_accessor a;
	*found = s_storage.find(a, key);
//	cout << hex << &s_storage << dec << " " << key << "? " << *found << endl;
	return *found ? a->second : istring::literal("");
}

void Storage::Set(istring key, istring val) {
	cout << hex << &s_storage << dec << " " << key << " -> " << val << endl;
	s_storage.insert({key, val});
}


void Storage::Delete(istring key) {
	s_storage.erase(key);
}