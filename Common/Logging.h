//
//  Logging.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include <strstream>

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
		extern const Color YELLOW;
		extern const Color CLEAR_ALL;
		
		class Logger {
		public:
			template <typename T>
			friend Logger operator<<(Logger&& logger, const T& rhs);
			
			template <typename T>
			friend Logger operator<<(Logger&& logger, const T* rhs);
			
			Logger(const Color& color, ostream& os = std::cout);
			Logger(Logger&& rhs);
			
		#if ENABLE_LOGGING
			~Logger();
			
		private:
			std::unique_ptr<ostringstream> buffer_ = make_unique<ostringstream>();
			ostream& os_ = std::cout;
			bool isLast_ = true;
		#endif
		};
		
		template <typename T>
		Logger operator<<(Logger&& logger, const T& rhs) {
		#if ENABLE_LOGGING
			*logger.buffer_ << rhs;
		#endif
			return std::move(logger);
		}
		
		template <typename T>
		Logger operator<<(Logger&& logger, const T* rhs) {
		#if ENABLE_LOGGING
			*logger.buffer_ << rhs;
		#endif
			return std::move(logger);
		}

	}
}