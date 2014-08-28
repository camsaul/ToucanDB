//
//  main.cpp
//  Money
//
//  Created by Cam Saul on 8/23/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Handler.h"

#include "Storage.h"

static const size_t kNumIterations = 50000;
using Clock = chrono::system_clock;

void Set(size_t) {
	Storage::Set("toucan", "rasta");
}

void Get(size_t) {
	bool found;
	Storage::Get("toucan", &found);
}

int main(int argc, const char * argv[])
{
	Storage::Set("toucan", "rasta");
	
	auto start = Clock::now();
	tbb::parallel_for(size_t(0), kNumIterations, Get);
	auto time = Clock::now() - start;
	
	auto mcs = chrono::duration_cast<chrono::microseconds>(time);
	auto ms2 = chrono::duration_cast<chrono::milliseconds>(time);
	
	cout << "total time: " << kNumIterations << " requests @ " << double(ms2.count())/1000.0 << " seconds" << endl;
	
	double avgTimePerIteration = mcs.count() / double(kNumIterations);
	cout << "avg time per itr: " << avgTimePerIteration << " microseconds" << endl;
		
	double itrsPerSecond = chrono::microseconds::period::den / avgTimePerIteration;
	cout << "itrsPerSecond: " << itrsPerSecond/1e+6  << " million" << endl;

	Handler();
}

