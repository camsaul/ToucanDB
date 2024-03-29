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
	
//	void Time(function<void()> fn) {
//		Time(1, fn);
//	}
	
	void Time(size_t numReps, function<void()> fn) {
		auto start = chrono::system_clock::now();
		for (size_t i = 0; i < numReps; i++) {
			fn();
		}
		auto time = chrono::system_clock::now() - start;
		auto ms = chrono::duration_cast<chrono::microseconds>(time);
		Logger(BLUE) << "Time: " << ms.count() << "µs";
	}
	
	void Time2(size_t numReps, function<void(size_t repNumber)> fn) {
		auto start = chrono::system_clock::now();
		for (size_t i = 0; i < numReps; i++) {
			fn(i);
		}
		auto time = chrono::system_clock::now() - start;
		auto ms = chrono::duration_cast<chrono::milliseconds>(time);
		Logger(BLUE) << "Time: " << ms.count() << "ms";
	}
}