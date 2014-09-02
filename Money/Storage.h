//
//  Storage.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
	class TString;
	class Storage {
	public:
		using KeyType = TString;
		using ValueType = TString;
		
		static	ValueType	Get		(KeyType key);
//		static	ValueType	Get		(KeyType key, bool* found);
		static	void		Set		(KeyType key, ValueType val);
		static	void		Delete	(KeyType key);
	};
}