//
//  Storage.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Storage.h"

namespace toucan_db {
	template <typename KeyType>
	struct KeyTraits {
		using StorageT = std::unordered_map<KeyType, Storage::ValueType>;
	};
	
	template<>
	struct KeyTraits<istring> {
		struct Hasher {
			inline static constexpr size_t hash(const istring& x ) {
				return x.hash();
			}
			
			inline static bool equal(const istring& x, const istring& y ) {
				return x == y;
			}
		};
		
		using StorageT = std::map<istring, Storage::ValueType>;
	};
	
	template<>
	struct KeyTraits<CamStr> {
		struct Hasher {
			inline static constexpr int64_t hash(const CamStr& x ) {
				return x.Hash();
			}
			
			inline static bool equal(const CamStr& x, const CamStr& y ) {
				return x == y;
			}
		};
		
		using StorageT = tbb::concurrent_hash_map<CamStr, Storage::ValueType, Hasher>;
	};
	
	using StorageT = KeyTraits<Storage::KeyType>::StorageT;
	static StorageT sStorage {};
	
	Storage::ValueType Storage::Get(KeyType key) {
		StorageT::const_accessor a;
		return sStorage.find(a, key) ? a->second : "";
//		StorageT::const_iterator itr;
//		return (itr = sStorage.find(key)) == sStorage.end() ? "" : itr->second;
	}
	
	void Storage::Set(KeyType key, ValueType val) {
		sStorage.insert({key, val});
		assert(!strcmp(Get(key), val));
//		cout << hex << &sStorage << ": " << dec << "SET '" << key << "' -> " << val << endl;
	}
	
	void Storage::Delete(KeyType key) {
		sStorage.erase(key);
	}
}



