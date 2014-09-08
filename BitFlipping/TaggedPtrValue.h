//
//  TaggedPtrValue.h
//  Money
//
//  Created by Cam Saul on 9/8/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include "Value.h"
#include "TaggedPtr2.h"

namespace toucan_db {
	template <DataType TypeTag, typename T, typename TagStructT>
	class TaggedPtrVal : public Value<TypeTag, TaggedPtr2<T, TagStructT>> {
	
		using ValueT		= Value<TypeTag, TaggedPtr2<T, TagStructT>>;
		using DataStructT	= typename ValueT::Data;
		
	protected:
		template <typename... ConstructorArgs>
			TaggedPtrVal(ConstructorArgs... args):
			ValueT(args...)
		{}
		
		TaggedPtrVal(const TaggedPtrVal&) = delete;
		
		TaggedPtrVal(TaggedPtrVal&& rhs) {
			Data().raw = rhs.Data().raw;
			if (this != &rhs) {
				rhs.Data().raw = 0;
			}
		}
		
		TaggedPtrVal& operator=(const TaggedPtrVal&) = delete;
		
		TaggedPtrVal& operator=(TaggedPtrVal&& rhs) {
			if (this != &rhs) {
				Data().raw = rhs.Data().raw;
				rhs.Data().raw = 0;
			}
			assert(ValueT::Type() == TypeTag);
			return *this;
		}
		
		~TaggedPtrVal() {} // subclass needs to take care of deleting
		
		DataStructT& Data()				{ return ValueT::data_; }
		const DataStructT& Data() const	{ return ValueT::data_; }
		
		TagStructT& Tag()				{ return Data().d.Tag(); }
		const TagStructT& Tag() const	{ return Data().d.Tag(); }
		
		const T* Ptr() const		{ return Data().d.Ptr(); }
		T* Ptr()					{ return Data().d.Ptr(); }
		void SetPtr(T* p)			{ Data().d.SetPtr(p); }
	};
}
