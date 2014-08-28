//
//  main.cpp
//  Money
//
//  Created by Cam Saul on 8/23/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Handler.h"
#include "logging.h"

//std::atomic<int> s_counter { 0 };

int main(int argc, const char * argv[])
{
	crow::logger::setLogLevel(crow::LogLevel::WARNING);
	Handler();
}

