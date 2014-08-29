//
//  Logging.cpp
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Logging.h"

namespace toucan_db {
	namespace logging {
		mutex sOutputMutex;
		
#pragma mark - Color

		Color::Color(const char* val):
			string(val)
		{}
		
		Color::Color(uint8_t r, uint8_t g, uint8_t b):
			string("\033[fg" + lexical_cast<string, int>(r) + "," + lexical_cast<string, int>(g) + "," + lexical_cast<string, int>(b) + ";")
		{}
		
		const Color RED		  = Color(160, 0, 0);
		const Color GREEN	  = Color(0, 160, 0);
		const Color BLUE	  = Color(0, 0, 160);
		const Color ORANGE	  = Color(204, 102, 0);
		const Color CLEAR_ALL = "\033[fg;";
		
#pragma mark - Logger

		Logger::Logger(const Color& color, ostream& os)
	#if ENABLE_LOGGING
		: os_ ( os )
		{
			Lock();
			os_ << color;
		}
	#else
		{}
	#endif
		
		Logger::Logger(Logger&& rhs)
	#if ENABLE_LOGGING
		: os_ (rhs.os_)
		{
			lock_.swap(rhs.lock_);
			if (this != &rhs) {
				rhs.isLast_ = false;
			}
		}
	#else
		{}
	#endif
		
	#if ENABLE_LOGGING
		Logger::~Logger() {
			if (isLast_) {
				os_ << CLEAR_ALL;
				os_ << endl;
			}
		}
		
		void Logger::Lock() {
			if (lock_.owns_lock()) return;
			lock_ = unique_lock<mutex>(sOutputMutex);
		}
	#endif
	}
}