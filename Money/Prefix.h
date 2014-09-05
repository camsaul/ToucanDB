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
#include <strstream>
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
#include <boost/bind.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/operators.hpp>
#include <boost/noncopyable.hpp>
#pragma clang diagnostic pop

#include <tbb/concurrent_hash_map.h>

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>

#include <protobuf-zerocopy-compression/compressed_stream.h>

#include <city.h>
#include <native/istring.h>

using namespace std;
using boost::lexical_cast;
using native::istring;

namespace toucan_db {
	static const string ƒ = "cool";
	/// We wil use suffix _64 for size_t literals since it's so important to the app
	constexpr size_t operator ""_p(unsigned long long num) { return (size_t)num; }
	#define x64 (size_t)
}