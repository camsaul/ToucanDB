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
	
	static const size_t kTagMask = 0xFFFF000000000000;
	static const size_t kPtrMask = ~kTagMask;
	
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
		
	private:
		UpperTagStruct	tag; // 16 bits
		size_t			ptr : 48;
	};
	
//	template <DataType Type, typename T, typename StructT>
//	class TaggedPtrVal : public Value<Type, TaggedPtr2<T, StructT>> {
//	
//	protected:
//		UpperTagStruct& Tag() { return data_.d.Tag(); }
//	};
}