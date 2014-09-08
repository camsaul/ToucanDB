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
	
	template <DataType TypeTag, class DataStruct>
	class Value {
		static_assert(sizeof(DataStruct) == 8, "DataStruct must be 8 bytes!");
	public:
		Value() = default;
		
		DataType Type() const {
			return static_cast<DataType>(data_.raw & 0b111);
		}
				
	protected:
		Value(size_t raw):
			data_(raw)
		{
			assert(Type() == TypeTag);
		}
		
		template<typename... ConstructorArgs>
		Value(ConstructorArgs... args):
			data_(args...)
		{
			assert(Type() == TypeTag);
		}

		union Data {
			DataStruct d;
			size_t raw;
			
			Data():
				raw (static_cast<size_t>(TypeTag))
			{}
			
			template<typename... ConstructorArgs>
			Data(ConstructorArgs... args):
				d(args...)
			{
				raw |= static_cast<size_t>(TypeTag);
			}
			
		} data_ = {};
	};
	
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
//
//		TaggedPtr2(UpperTagStruct t, T* p):
//			upperTag(t)
//		{
//			SetPtr(p);
//		}
//		
//		TaggedPtr2(UpperTagStruct t):
//			upperTag(t)
//		{}
		
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
	
	template <DataType TypeTag, typename T, typename StructT>
	class TaggedPtrVal : public Value<TypeTag, TaggedPtr2<T, StructT>> {
	using ValueT = Value<TypeTag, TaggedPtr2<T, StructT>>;
	
	protected:
		template <typename... ConstructorArgs>
		TaggedPtrVal(ConstructorArgs... args):
			ValueT(args...)
		{}
	
		StructT& Tag()				{ return ValueT::data_.d.Tag(); }
		const StructT& Tag() const	{ return ValueT::data_.d.Tag(); }
		
		const T* Ptr() const		{ return ValueT::data_.d.Ptr(); }
		T* Ptr()					{ return ValueT::data_.d.Ptr(); }
		void SetPtr(T* p)			{ ValueT::data_.d.SetPtr(p); }
	};
}