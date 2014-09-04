//
//  BitFlipUtils.h
//  Money
//
//  Created by Cam Saul on 9/4/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include "Logging.h"

namespace toucan_db {
	namespace bit_flip {
		using namespace toucan_db::logging;
		
#pragma mark - IsRedBit()
		/// Is this bit unused for storage by pointers?
		auto IsRedBit = [](auto i) -> bool {
			return i >= 48 || i < 3;
		};
		
#pragma mark - BitAt<PtrT, IdxT>
		/// Get whether the bit at a specific index is true / false
		template <typename PtrT, typename IntT = size_t>
		constexpr bool BitAt(PtrT ptr, IntT i) {
			return static_cast<size_t>(ptr) & ((size_t)1 << static_cast<size_t>(i));
		}
		
#pragma mark - DumpBinary<T>
		/// Helper struct to dump binary representation of a 64-bit type.
		template <typename T>
		struct DumpBinary {
			static_assert(is_integral<T>::value, "T must be an integral type!");
			static_assert(sizeof(T) == sizeof(void*), "T must be 64 bits!");
		private:
			T val_;
		public:
			DumpBinary(T val):
				val_ (val)
			{}
			
			template <typename X, typename = typename enable_if<is_convertible<X, T>::value>::type>
			DumpBinary(X val):
				val_ (static_cast<T>(val))
			{}
			
			template <typename X, typename = typename enable_if<!is_same<X*, T>::value>::type>
			DumpBinary(X* val):
				val_ (reinterpret_cast<T>(val))
			{}
			
			DumpBinary(const DumpBinary&) = delete;
			DumpBinary(DumpBinary&&) = delete;
			
			friend ostream& operator<<(ostream& os, const DumpBinary& dumper) {
				static auto lastColor = BLUE;
				auto getBitAt = bind(BitAt<T>, dumper.val_, placeholders::_1);
				
				for (auto i = 63; i >= 0; i--) {
					auto newColor = IsRedBit(i) ? RED : GREEN;
					if (lastColor != newColor) {
						os << newColor;
						lastColor = newColor;
					}
					os << (getBitAt(i) ? '1' : '0');
					if (!(i % 8)) os << ' ';
				}
				os << BLUE << "0x" << hex << setw(16) << setfill('0') << right << dumper.val_ << YELLOW << " " << dec << dumper.val_ << RED;
				return os;
			}
		};
		
#pragma mark - UniqueCPtr<T, DefaultSize>
		template <typename T, size_t DefaultSize = sizeof(T)>
		struct UniqueCPtr {
			
			static_assert(DefaultSize >= sizeof(T), "Requested size must be greated than sizeof(T)!");
			static_assert(DefaultSize % sizeof(T) == 0, "Requested size must be a multiple of sizeof(T)!");
			static_assert(is_pod<T>::value, "T must be POD!");
			
			constexpr UniqueCPtr(nullptr_t):
				ptr_ (nullptr)
			{}
			
			constexpr UniqueCPtr(size_t size = DefaultSize):
				ptr_ ((T*)malloc(size))
			{}
			
			/// Take over management of a pointer (e.g., created with malloc)
			constexpr UniqueCPtr(T* ptr):
				ptr_ (ptr)
			{};
			
			UniqueCPtr(UniqueCPtr&& rhs):
			ptr_(rhs.ptr_)
			{
				if (this != &rhs) {
					rhs.ptr_ = nullptr;
				}
			}
			
			UniqueCPtr(const UniqueCPtr& rhs) = delete;
			UniqueCPtr& operator=(const UniqueCPtr& rhs) = delete;
			
			operator bool()       const { return ptr_ != nullptr; }
			T&		 operator*()		{ return *ptr_; }
			const T& operator*()  const { return *ptr_; }
			T&		 operator->()		{ return *ptr_; }
			const T& operator->() const { return *ptr_; }
			
			~UniqueCPtr() {
				free(ptr_);
			}
			
			friend ostream& operator<<(ostream& os, const UniqueCPtr& rhs) {
				os << DumpBinary<ssize_t>(rhs.ptr_);
				return os;
			}
		private:
			T* ptr_;
		};

	}
}