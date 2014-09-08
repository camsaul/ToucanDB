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
		char str[7];   // 56
		size_t size		 : 4;
		size_t			 : 1; // unused
		DataType type    : 3;
	};
	
	class ShortString : public Value<DataType::SHORT_STR, ShortStringData> {
	public:
		ShortString(const char* str) {
			for (int i = 0; i < 7; i++) {
				char c = str[i];
				if (c == 0) {
					data_.d.size = i;
					return;
				}
				data_.d.str[i] = c;
			}
			data_.d.size = 7;
		}
		
		size_t Length() const {
			return data_.d.size;
		}
	};

	static_assert(sizeof(ShortString) == 8, "ShortString should be 64 bits!");
}