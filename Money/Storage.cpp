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

#define USE_CONCURRENT_HASH_MAP 1

#if USE_CONCURRENT_HASH_MAP
	using StorageT = tbb::concurrent_hash_map<istring, istring, istring_hash>;
#else
	using StorageT = unordered_map<istring, istring>;
#endif

static StorageT s_storage {};

#if USE_CONCURRENT_HASH_MAP
	istring Storage::Get(istring key, bool* found) {
		StorageT::const_accessor a;
		return std::move((*found = s_storage.find(a, key)) ? a->second : istring::literal(""));
	}

	void Storage::Set(istring key, istring val) {
//		cout << hex << &s_storage << dec << " " << key << " -> " << val << endl;
		s_storage.insert({key, val});
	}


	void Storage::Delete(istring key) {
		s_storage.erase(key);
	}
#else
	istring Storage::Get(istring key, bool* found) {
		const auto itr = s_storage.find(key);
		if (itr == s_storage.end()) {
			*found = false;
			return istring::literal("");
		} else {
			*found = true;
			return itr->second;
		}
	}

	void Storage::Set(istring key, istring val) {
//		cout << hex << &s_storage << dec << " " << key << " -> " << val << endl;
		s_storage[key] = val;
	}


	void Storage::Delete(istring key) {
		s_storage.erase(key);
	}
#endif