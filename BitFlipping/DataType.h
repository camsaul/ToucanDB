//
//  DataType.h
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
	enum class DataType : size_t {
		NUL			= 0b000,
		LONG_STR	= 0b001,
		SHORT_STR	= 0b010,
		INT			= 0b011, ///< Or boolean (?)
		DOUBLE		= 0b100,
		SHORT_ARRAY	= 0b101,
		LONG_ARRAY	= 0b110,
		DICT		= 0b111
	};
}