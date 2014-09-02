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
		using KeyType   = istring;
		using ValueType = istring;
		
		static	const ValueType&	Get		(const KeyType& key);
		static	void				Set		(const KeyType& key, ValueType&& val);
		static	void				Delete	(const KeyType& key);
		
	private:
		static void RehashIfNeeded();
	};
}