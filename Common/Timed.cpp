//
//  Timer.cpp
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Timed.h"
#include "Logging.h"

using namespace toucan_db::logging;

namespace toucan_db {
	Timed::Timed(std::function<void()> fn) {
		auto start = chrono::system_clock::now();
		
		fn();
		
		auto time = chrono::system_clock::now() - start;
		auto ms = chrono::duration_cast<chrono::microseconds>(time);
		Logger(BLUE) << "Time: " << ms.count() << "µs";
	}
}