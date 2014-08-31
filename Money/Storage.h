//
//  Storage.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
	class Storage {
	public:
		static atomic<int> sNumGets;
		
		static	istring Get		(istring key, bool* found);
		static	void	Set		(istring key, istring val);
		static	void	Delete	(istring key);
	private:
		struct istring_hash {
			inline static constexpr size_t hash(const istring& x ) {
				return x.hash();
			}
			
			inline static bool equal(const istring& x, const istring& y ) {
				return x == y;
			}
		};

		using StorageT = tbb::concurrent_hash_map<istring, istring, istring_hash>;
		static StorageT sStorage;
	};
}