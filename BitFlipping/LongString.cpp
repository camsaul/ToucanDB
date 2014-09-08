//
//  LongString.cpp
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "LongString.h"

namespace toucan_db {
	ostream& operator<<(ostream& os, const LongString& str) {
		os << str.Ptr();
		return os;
	}
}