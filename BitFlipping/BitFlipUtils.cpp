//
//  BitFlipUtils.cpp
//  Money
//
//  Created by Cam Saul on 9/4/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "BitFlipUtils.h"

namespace toucan_db {
	namespace bit_flip {
		// BitAt tests
		static_assert(BitAt(0b0000, 0) == false, "BitAt error!");
		static_assert(BitAt(0b0001, 0) == true, "BitAt error!");
		static_assert(BitAt(0b0000, 1) == false, "BitAt error!");
		static_assert(BitAt(0b0010, 1) == true, "BitAt error!");
		static_assert(BitAt(0b0000, 2) == false, "BitAt error!");
		static_assert(BitAt(0b0100, 2) == true, "BitAt error!");
		static_assert(BitAt(0b0000, 3) == false, "BitAt error!");
		static_assert(BitAt(0b1000, 3) == true, "BitAt error!");
		static_assert(BitAt(0b000000000, 8) == false, "BitAt error!");
		static_assert(BitAt(0b100000000, 8) == true, "BitAt error!");
		static_assert(BitAt(0, 32) == false, "BitAt error!");
	}
}