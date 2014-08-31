//
//  Logging.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#define ENABLE_LOGGING 1

namespace toucan_db {
	namespace logging {
		class Color : public string {
		public:
			Color(const char* val);
			Color(uint8_t r, uint8_t g, uint8_t b);
		};
		extern const Color RED;
		extern const Color GREEN;
		extern const Color BLUE;
		extern const Color ORANGE;
		extern const Color CLEAR_ALL;
		
		class Logger {
		public:
//			template <typename T, typename, typename TRef>
//			friend Logger operator<<(Logger&& logger, const TRef rhs);
			
//			template <typename T>
//			friend Logger operator<<(Logger&& logger, T&&rhs);
			
			template <typename T>
			friend Logger operator<<(Logger&& logger, const T& rhs);
			
			template <typename T>
			friend Logger operator<<(Logger&& logger, const T* rhs);
			
			Logger(const Color& color, ostream& os = std::cout);
			Logger(Logger&& rhs);
			
		#if ENABLE_LOGGING
			~Logger();
			
		private:
			void Lock();
			
			unique_lock<mutex> lock_;
			ostream& os_ = std::cout;
			bool isLast_ = true;
		#endif
		};
		
//		template <typename T, typename = typename enable_if<is_object<T>::value>::type, typename TRef = typename add_lvalue_reference<T>::type>
//		Logger operator<<(Logger&& logger, const TRef rhs) {
//			#if ENABLE_LOGGING
//				logger.os_ << rhs;
//			#endif
//			return std::move(logger);
//		}
		
//		template <typename T>
//		Logger operator<<(Logger&& logger, T&& rhs) {
//			#if ENABLE_LOGGING
//				logger.os_ << rhs;
//			#endif
//			return std::move(logger);
//		}
		
		template <typename T>
		Logger operator<<(Logger&& logger, const T& rhs) {
		#if ENABLE_LOGGING
			logger.os_ << rhs;
		#endif
			return std::move(logger);
		}
		
		template <typename T>
		Logger operator<<(Logger&& logger, const T* rhs) {
		#if ENABLE_LOGGING
			logger.os_ << rhs;
		#endif
			return std::move(logger);
		}

	}
}