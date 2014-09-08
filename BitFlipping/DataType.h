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
		SHORT_STR	= 0b001,
		LONG_STR	= 0b010,
		INT			= 0b011, ///< Or boolean (?)
		DOUBLE		= 0b100,
		SHORT_ARRAY	= 0b101,
		LONG_ARRAY	= 0b110,
		DICT		= 0b111
	};
	
	template <DataType Type, class DataStruct>
	class Value {
		static_assert(sizeof(DataStruct) == 8, "DataStruct must be 8 bytes!");
	public:
		Value() = default;
		
		Value(size_t raw):
			data_(raw)
		{}
		
//		Value(DataStruct d):
//			data_(d)
//		{}
		
		template<typename... ConstructorArgs>
		Value(ConstructorArgs... args):
			data_(args...)
		{}
		
	protected:
		union Data {
			DataStruct d;
			size_t raw;
			
			Data(size_t r):
				raw(r)
			{}
			
			template<typename... ConstructorArgs>
			Data(ConstructorArgs... args):
				d(args...)
			{}
			
		} data_ = (static_cast<size_t>(Type));
	};
	
	static const size_t kUpperTagMask		= 0xFFFF000000000000;
	static const size_t kLowerTagMask		= 0b0111;
	static const size_t kPtrMask			= ~(kUpperTagMask|kLowerTagMask); ///< Get just the pointer. Still need to fix top two bytes
	static const size_t kSignificantBitMask = 0x0000800000000000; // get the most significant bit
	
	template <typename T, class UpperTagStruct>
	class TaggedPtr2 {
		static_assert(sizeof(T*) == 8, "T* must be 4 bytes!");
		static_assert(sizeof(UpperTagStruct) == 2, "Upper tag must be exactly 2 bytes!");
	public:
		TaggedPtr2():
			ptr(0)
		{}
		
		TaggedPtr2(T* p):
			TaggedPtr2({}, p)
		{}
				
		TaggedPtr2(UpperTagStruct t, T* p):
			tag(t),
			ptr(reinterpret_cast<size_t>(p) & kPtrMask)
		{}
		
		const UpperTagStruct& Tag() const { return tag; }
		UpperTagStruct& Tag() { return tag; }
		
		const T* Ptr() const {
			size_t p = ptr & kPtrMask;
			if (kSignificantBitMask & ptr) {
				p |= kUpperTagMask;
			}
			return reinterpret_cast<T*>(p);
		}
		
	private:
		UpperTagStruct	tag; // 16 bits
		size_t			ptr : 48;
	};
	
	template <DataType Type, typename T, typename StructT>
	class TaggedPtrVal : public Value<Type, TaggedPtr2<T, StructT>> {
	using ValueT = Value<Type, TaggedPtr2<T, StructT>>;
	
	protected:
		template <typename... ConstructorArgs>
		TaggedPtrVal(ConstructorArgs... args):
			ValueT(args...)
		{}
	
		StructT& Tag()				{ return ValueT::data_.d.Tag(); }
		const StructT& Tag() const	{ return ValueT::data_.d.Tag(); }
		
		const T* Ptr() const		{ return ValueT::data_.d.Ptr(); }
	};
}