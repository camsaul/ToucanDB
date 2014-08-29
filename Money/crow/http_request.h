#pragma once

#include "common.h"

namespace crow
{
    struct request
    {
        HTTPMethod method;
		istring url;
        istring body;
		
//		request() {
//			cout << "request()" << endl;
//		}
		
		~request() {
			cout << "~request()" << endl;
		}
    };
}
