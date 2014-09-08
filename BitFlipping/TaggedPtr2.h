//
//  TaggedPtr2.h
//  Money
//
//  Created by Cam Saul on 9/8/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
	static const size_t kUpperTagMask		= 0xFFFF000000000000;
	static const size_t kLowerTagMask		= 0b0111;
	static const size_t kStripUpperTagMask	= ~kUpperTagMask;
	static const size_t kPtrMask			= ~(kUpperTagMask|kLowerTagMask); ///< Get just the pointer. Still need to fix top two bytes
	static const size_t kSignificantBitMask = 0x0000800000000000; // get the most significant bit
	
	template <typename T, class UpperTagStruct>
	class TaggedPtr2 {
		static_assert(sizeof(T*) == 8, "T* must be 4 bytes!");
		static_assert(sizeof(UpperTagStruct) == 2, "Upper tag must be exactly 2 bytes!");
	public:
		TaggedPtr2() = default;
		
		TaggedPtr2(const TaggedPtr2&) = delete;
		TaggedPtr2(TaggedPtr2&& rhs):
			lowerTag(rhs.lowerTag),
			ptr(rhs.ptr),
			upperTag(rhs.upperTag)
		{
			if (this != &rhs) {
				rhs.ptr = 0;
			}
		}
		
		~TaggedPtr2() {}
		
		const UpperTagStruct& Tag() const { return upperTag; }
		UpperTagStruct& Tag()			  { return upperTag; }
		
		const T* GetPtr() const {
			size_t p = ptr << 3;
			p &= kPtrMask;
			if (kSignificantBitMask & ptr) {
				p |= kUpperTagMask;
			}
			return reinterpret_cast<T*>(p);
		}
		
		const T* Ptr() const { return GetPtr(); }
		T* Ptr()			 { return const_cast<T*>(GetPtr()); }
		
		void SetPtr(T* p) {
			ptr = (reinterpret_cast<size_t>(p) & kPtrMask) >> 3;
		}
		
	private:
		size_t			lowerTag : 3;
		size_t			ptr		 : 45;
		UpperTagStruct	upperTag; // 16 bits
	};
}