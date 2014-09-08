//
//  TaggedPtr.h
//  Money
//
//  Created by Cam Saul on 9/4/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include "Logging.h"
#include "BitFlipUtils.h"

namespace toucan_db {
namespace bit_flip {
	
	using namespace toucan_db::logging;
	
	class TaggedPtr {
	public:
		struct Tags {
			size_t lowerTag				:   3;
			size_t						:  44; ///< Unused
			size_t mostSignificantBit	:   1;
			size_t upperTag				:  16;
		};
		
		inline constexpr TaggedPtr(size_t original):
			value { .ptr = original }
		{}
		
		constexpr size_t GetUpper() const {
			return value.tags.upperTag;
		}
		
		TaggedPtr SetUpper(size_t upper) const {
			return value.ptr;
		}
		
		constexpr size_t GetLower() const {
			return value.ptr & kLowerTagMask;
		}
		
		TaggedPtr SetLower(size_t lower) const {
			return value.ptr;
		}
		
		constexpr size_t GetTagged() const {
			return value.ptr;
		}
		
		/// Convert to pointer that can be used in real lyfe $$
		constexpr size_t GetUntagged() const {
			if (!value.tags.mostSignificantBit) {
				return value.ptr & kStripTagsMask[0];
			} else {
				return (value.ptr & kStripTagsMask[1]) | kUpperTagMask; // top 2 bytes must be all '1's
			}
		}
		
	private:
		static const size_t kStripTagsMask[2]; ///< index off of most significant bit
		static const size_t kMostSignificantBitMask;
		static const size_t kUpperTagMask;
		static const size_t kLowerTagMask;
		
		const union {
			const size_t ptr;
			const Tags tags;
		} value;
	};
	
	static_assert(sizeof(TaggedPtr) == 8, "TaggedPtr is > 64 bits!!");
}
}