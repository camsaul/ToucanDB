//
//  Storage.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include "city.h"

namespace toucan_db {
	class CamStr {
	public:
		CamStr(const char* s = nullptr) noexcept:
			str(s)
		{}
		
		CamStr(const CamStr& rhs) noexcept:
			str  (rhs.str)
		{}
		
		CamStr& operator=(const CamStr& rhs) noexcept {
			str   = rhs.str;
			return *this;
		}
		
		CamStr(CamStr&& rhs) noexcept :
			str  (rhs.str)
		{}
		
		CamStr& operator=(CamStr&& rhs) noexcept {
			str   = rhs.str;
			return *this;
		}
		
		inline operator bool() const noexcept {
			return str != nullptr;
		}
		
		inline bool operator==(const CamStr& rhs) const noexcept {
			return !strcmp(str, rhs.str);
		}
		
		inline bool operator<(const CamStr& rhs) const noexcept {
			return strcmp(str, rhs.str) < 0;
		}
		
		inline uint64 Hash() const noexcept {
			return CityHash64(str, strlen(str));
		}
		
	private:
//		struct InlineData {
//			uint64_t unused		   : 56;
//			uint8_t  refCount      : 7;
//			bool	 isInline	: 1;
//		};
//		union Data {
//			InlineData inlineData;
//			uint64_t*  data = nullptr;
//		};
//		Data data_;
		const char *str = nullptr;
	};
	
	
	
	class Storage {
	public:
		using KeyType = CamStr;
		using ValueType = const char *;
		
		static	ValueType	Get		(KeyType key);
//		static	ValueType	Get		(KeyType key, bool* found);
		static	void		Set		(KeyType key, ValueType val);
		static	void		Delete	(KeyType key);
	};
	
	static const Storage::KeyType sKeys[] {
		"toucan_0",
		"toucan_1",
		"toucan_2",
		"toucan_3",
		"toucan_4",
		"toucan_5",
		"toucan_6",
		"toucan_7",
		"toucan_8",
		"toucan_9",
		"toucan_10",
		"toucan_11",
		"toucan_12",
		"toucan_13",
		"toucan_14",
		"toucan_15",
		"toucan_16",
		"toucan_17",
		"toucan_18",
		"toucan_19",
		"toucan_20",
		"toucan_21",
		"toucan_22",
		"toucan_23",
		"toucan_24",
		"toucan_25",
		"toucan_26",
		"toucan_27",
		"toucan_28",
		"toucan_29",
		"toucan_30",
		"toucan_31",
	};
}