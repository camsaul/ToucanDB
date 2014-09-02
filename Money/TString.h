//
//  String.h
//  Money
//
//  Created by Cam Saul on 9/2/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once 

namespace toucan_db {
	static const unsigned char kInline[8] = { 0xFF, 0, 0, 0, 0, 0, 0, 0 };
	static const uint64_t	   kInlineRaw = 0xFF;
	
	class TString {
	public:
		TString() noexcept = default;
		
		TString(const char* s) noexcept:
			TString(s, strlen(s))
		{}
		
		TString(const char* s, size_t sLen) noexcept
		{
			if (sLen <= 6) {
				
				data_.inl[0] = 0xFF;
				strncpy(data_.inl + 1, s, 6);
			} else {
				int64_t hash = CityHash64(s, sLen);
								
				sLen++; // save room for null-byte
				sLen += 8 - (sLen % 8); // round up to nearest size divisible by 8
				
				auto storage = (char *)malloc(sLen);
				strncpy(storage, s, sLen);
				
				data_.ptr = new shared_ptr<ExternalData>;
				*data_.ptr = make_shared<ExternalData>(hash, sLen, storage);
			}
		}
		
		~TString() {
			if (!IsInline()) {
				delete data_.ptr;
				data_.ptr = nullptr;
			}
		}
		
		TString(const TString& rhs) noexcept = default;
		TString(TString&& rhs) noexcept = default;
		
		TString& operator=(const TString& rhs) noexcept = default;
		TString& operator=(TString&& rhs) noexcept = default;
		
		operator bool() const noexcept {
			return data_.raw != kInlineRaw;
		}
		
		explicit operator const char*() const noexcept {
			return IsInline() ? static_cast<const char*>(data_.inl + 1) : (*data_.ptr)->storage;
		}
		
		inline bool operator==(const TString& rhs) const noexcept {
			if (IsInline()) {
				if (!rhs.IsInline()) return false;
				return data_.raw == rhs.data_.raw;
			} else {
				const auto& data = **data_.ptr;
				const auto& rhsData = **rhs.data_.ptr;
				
				const auto length = data.length;
				assert(!(length % 8));
				if (length != rhsData.length) return false;
				
				const auto numQuadWords = length / 8;
				
				auto storage = reinterpret_cast<const int64_t*>(data.storage);
				auto rhsStorage = reinterpret_cast<const int64_t*>(rhsData.storage);
				
				for (size_t i = 0; i < numQuadWords; i++) {
					if (storage[i] != rhsStorage[i]) return false;
				}
				return true;
			}
		}
		
		inline uint64 Hash() const noexcept {
			return IsInline() ? CityHash64(reinterpret_cast<const char*>(data_.inl + 1), 7) : (*data_.ptr)->hash;
			}
			
			inline bool IsInline() const {
				return data_.inl[0] == 0xFF;
			}
			
		private:
			struct ExternalData {
				const int64_t hash;
				const size_t  length;
				const char*	  storage;
				
				ExternalData(int64_t hsh, size_t len, char* str):
					hash(hsh), length(len), storage(str)
				{}
				ExternalData(const ExternalData&) = delete;
				ExternalData(ExternalData&&)	  = default;
				
				ExternalData& operator=(const ExternalData& rhs) noexcept = delete;
				ExternalData& operator=(ExternalData&& rhs) noexcept      = default;
				
				~ExternalData() {
					delete[] storage;
				}
			};
			union Data {
				uint64_t					raw;
				unsigned char				inl[8];
				shared_ptr<ExternalData>*	ptr;
				
				Data() noexcept
				{
					raw = 0;
					inl[0] = 0xFF;
				}
			};
			
			Data data_;
		};
}