//
//  Logging.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
	namespace logging {
		class Color : public string {
		public:
			Color(const char* val);
		};
		extern const Color RED;
		extern const Color GREEN;
		extern const Color BLUE;
		extern const Color CLEAR_ALL;
		
		class Logger {
		public:
			template <typename T>
			friend Logger operator<<(Logger&& logger, T rhs);
			
			Logger(const Color& color, ostream& os = std::cout);
			Logger(Logger&& rhs);
			
			~Logger();
			
		private:
			void Lock();
			
			unique_lock<mutex> lock_;
			ostream& os_ = std::cout;
			bool isLast_ = true;
		};
		
		template <typename T>
		Logger operator<<(Logger&& logger, T rhs) {
			logger.os_ << rhs;
			return std::move(logger);
		}

	}
}