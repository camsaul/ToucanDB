//
//  Prefix.h
//  Money
//
//  Created by Cam Saul on 8/24/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>
#include <unordered_map>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconditional-uninitialized"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/operators.hpp>
#include <boost/noncopyable.hpp>
#pragma clang diagnostic pop

#include <tbb/concurrent_hash_map.h>
#include <tbb/parallel_for.h>

#include <native/istring.h>

#include "http_parser_merged.h"


using namespace std;
using boost::lexical_cast;
using native::istring;