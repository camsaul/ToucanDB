//
//  Timer.h
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
//	template <typename IntervalT = std::chrono::microseconds>
	class Timed {
	public:
		Timed(std::function<void()> fn);
	};
}