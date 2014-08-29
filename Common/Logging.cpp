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
		
		const Color RED		  = "\033[fg160,0,0;";	// "\e[1;31m";
		const Color GREEN	  = "\033[fg0,160,0;";	// "\e[1;32m";
		const Color BLUE	  = "\033[fg0,0,160;";	// "\e[1;32m";
		const Color CLEAR_ALL = "\033[fg;";			// "\e[0m";
		
#pragma mark - Logger

		Logger::Logger(const Color& color, ostream& os):
			os_ ( os )
		{
			Lock();
			os_ << color;
		}
		
		Logger::Logger(Logger&& rhs):
			os_ (rhs.os_)
		{
			lock_.swap(rhs.lock_);
			if (this != &rhs) {
				rhs.isLast_ = false;
			}
		}
		
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
	}
}