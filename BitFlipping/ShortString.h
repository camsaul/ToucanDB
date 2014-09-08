//
//  ShortString.h
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include "DataType.h"

namespace toucan_db {
	struct ShortStringData {
		DataType type    : 3;
		size_t			 : 1;
		size_t size		 : 4;
		char str[7];   // 56
	};
	
	class ShortString : public Value<DataType::SHORT_STR, ShortStringData> {
	public:
		ShortString(const char* str);
		
		size_t Length() const {
			return data_.d.size;
		}
		
		friend ostream& operator<<(ostream& os, const ShortString& str);
	};

	static_assert(sizeof(ShortString) == 8, "ShortString should be 64 bits!");
}