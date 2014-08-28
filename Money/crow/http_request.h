#pragma once

#include "common.h"

namespace crow
{
    struct request
    {
        HTTPMethod method;
        istring url;
//        std::unordered_map<std::string, std::string> headers;
        istring body;
    };
}
