//
//  Timer.h
//  Money
//
//  Created by Cam Saul on 9/7/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
//	void Time(function<void()>);
	
	void Time(size_t numReps, function<void()>);
	void Time2(size_t numReps, function<void(size_t repNumber)>);
	static const auto Timed1Million = bind(&Time2, 1000000_p, placeholders::_1);
}