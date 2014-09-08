//
//  Value.h
//  Money
//
//  Created by Cam Saul on 9/8/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include "DataType.h"

namespace toucan_db {
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
		
		template <typename T = DataStruct, typename = typename enable_if<is_copy_constructible<T>::value>::type>
		Value(const Value& rhs):
			data_(rhs.data_)
		{}
		
		template <typename T = DataStruct, typename = typename enable_if<!is_copy_constructible<T>::value>::type>
		Value(Value&& rhs):
			data_(std::move(rhs.data_))
		{}
		
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
			
			template <typename T = DataStruct, typename = typename enable_if<is_copy_constructible<T>::value>::type>
			Data(const Data& rhs):
				raw(rhs.raw)
			{}
			
			template <typename T = DataStruct, typename = typename enable_if<!is_copy_constructible<T>::value>::type>
			Data(Data&& rhs):
				raw(rhs.raw)
			{
				if (this != &rhs) {
					rhs.raw = 0;
				}
			}
			
			template<typename... ConstructorArgs>
			Data(ConstructorArgs... args):
				d(args...)
			{
				raw |= static_cast<size_t>(TypeTag);
			}
			
			~Data() {}
			
		} data_ = {};
	};
}