//
//  Storage.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Storage.h"
#include "Logging.h"

using namespace toucan_db::logging;
static atomic<size_t> sNextRehashInterval { 4 };
static atomic<size_t> sSetsBeforeNextRehash { 4 } ;

namespace toucan_db {
	template <typename KeyType>
	struct KeyTraits {
		using StorageT = tbb::concurrent_hash_map<KeyType, Storage::ValueType>;
	};
	
	template<>
	struct KeyTraits<istring> {
		struct Hasher {
			inline static constexpr int64_t hash(const istring& x ) {
				return x.hash();
			}
			
			inline static bool equal(const istring& x, const istring& y ) {
				return x == y;
			}
		};
		
		using StorageT = tbb::concurrent_hash_map<istring, Storage::ValueType, Hasher>;
	};
	
	template<>
	struct KeyTraits<const char*> {
		struct Hasher {
			inline static int64_t hash(const char* x) {
				return CityHash64(x, strlen(x));
			}
			
			inline static bool equal(const char* x, const char* y ) {
				return !(strcmp(x, y));
			}
		};
		
		using StorageT = tbb::concurrent_hash_map<const char*, Storage::ValueType, Hasher>;
	};
	
	template <typename ValueType>
	struct ValueTraits {
		static const ValueType kNullValue = nullptr;
	};
	
	template <>
	struct ValueTraits<istring> {
		static const istring kNullValue;
	};
	const istring ValueTraits<istring>::kNullValue = istring::literal("");
	
	using StorageT = KeyTraits<Storage::KeyType>::StorageT;
	auto kNullValue = ValueTraits<Storage::ValueType>::kNullValue;
	
	static StorageT sStorage {};
	
	Storage::ValueType Storage::Get(KeyType key) {
		StorageT::const_accessor a;
		return sStorage.find(a, key) ? a->second : kNullValue;
	}
	
	void Storage::Set(KeyType key, ValueType val) {
		StorageT::accessor a;
		if (sStorage.find(a, key)) {
			a->second = val;
		} else {
			sStorage.insert({key, val});
			if (!--sSetsBeforeNextRehash) {
				sNextRehashInterval = sNextRehashInterval * 2;
				sSetsBeforeNextRehash = (size_t)sNextRehashInterval;
				Logger(ORANGE) << "Rehashing..." << (size_t)sSetsBeforeNextRehash;
				sStorage.rehash();
			}
		}
	}
	
	void Storage::Delete(KeyType key) {
		sStorage.erase(key);
	}
}



