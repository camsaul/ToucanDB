//
//  TaggedPtr.cpp
//  Money
//
//  Created by Cam Saul on 9/4/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "TaggedPtr.h"

namespace toucan_db {
namespace bit_flip {
	const size_t TaggedPtr::kMostSignificantBitMask = 1_p << 63_p;
	const size_t TaggedPtr::kUpperTagMask = (0x8FFF_p << 46_p) - kMostSignificantBitMask;
	const size_t TaggedPtr::kLowerTagMask = 0b0111_p;
	
	const size_t TaggedPtr::kStripTagsMask[] {
		~(kMostSignificantBitMask | kUpperTagMask | kLowerTagMask), // Most-significant 16 should all be 0
		~kLowerTagMask
	};
}
}