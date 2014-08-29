//
//  main.cpp
//  CLI
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Logging.h"
#include "Client.h"

using namespace toucan_db;
using namespace toucan_db::logging;

int main(int argc, const char * argv[])
{
	Logger(BLUE) << "HERE!";
	
	auto client = Client();
	while (true) {
		client.Connect();
	}
    return 0;
}

