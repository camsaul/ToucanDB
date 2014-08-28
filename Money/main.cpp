//
//  main.cpp
//  Money
//
//  Created by Cam Saul on 8/23/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "crow.h"
#include "Node.h"
#include "RunLoop.h"

int main(int argc, const char * argv[])
{
	Key::Init();
	
	crow::Crow app;
	
	CROW_ROUTE(app, "/").name("hello")
    ([]{
        return "Hello World!";
    });
	
	crow::logger::setLogLevel(crow::LogLevel::DEBUG);
	
	app.port(1373).multithreaded().run();
//	
//	RunLoop();
}

