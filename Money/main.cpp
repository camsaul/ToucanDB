//
//  main.cpp
//  Money
//
//  Created by Cam Saul on 8/23/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "crow.h"
#define USE_C_STRS 1
#define USE_NATIVE_STRS 0
#define USE_STD_STRS 0

// 123x/158x
#if USE_C_STRS
using StringType = const char*;
using Storage = tbb::concurrent_hash_map<StringType, StringType>;
#endif

// 100x/110x
#if USE_NATIVE_STRS
using StringType = native::istring;
struct MyHashCompare {
    inline static size_t hash(const StringType& x ) {
		return x.hash();
    }
    //! True if strings are equal
    inline static bool equal( const StringType& x, const StringType& y ) {
        return x==y;
    }
};
using Storage = tbb::concurrent_hash_map<StringType, StringType, MyHashCompare>;
#endif

// 80x/84x
#if USE_STD_STRS
using StringType = std::string;
using Storage = tbb::concurrent_hash_map<StringType, StringType>;
#endif


std::atomic<int> s_counter { 0 };

#define REUSE_ACCESSOR 0

class MyStorage {
	static Storage storage_;
public:
	class Accessor {
	public:
		static inline void Set(StringType key, StringType val) {
			storage_.insert({key, val});
		}
		static inline StringType Get(StringType key, bool* found) {
			Storage::const_accessor a_;
			
			*found = storage_.find(a_, key);
			return *found ? a_->second : "";
		}
		
		static inline void Delete(StringType key) {
			storage_.erase(key);
		}
	};
	MyStorage() {
		cout << "MyStorage()" << endl;
	}
	~MyStorage() {
		std::cout << "~MyStorage()" << std::endl;
	}
};

Storage MyStorage::storage_ {};


int main(int argc, const char * argv[])
{
	crow::Crow app;
	
	CROW_ROUTE(app, "/<string>").name("hello")
    ([](const crow::request& req, crow::response& res, const string& key){
		switch (req.method) {
			case crow::HTTPMethod::GET: {
				cout << "GET: " << key << endl;
				bool found;
				if (!found) res.code = 404;
				res.end(MyStorage::Accessor::Get(key.c_str(), &found));
				cout << "FOUND? " << found << endl;
				return;
			} break;
			case crow::HTTPMethod::PUT:
			case crow::HTTPMethod::POST: {
				cout << "SET: " << key << " -> " << req.body.c_str() << endl;
				MyStorage::Accessor::Set(key.c_str(), req.body.c_str());
				res.code = 201;
				res.body = "ok";
				res.end();
				return;
			} break;
			case crow::HTTPMethod::DELETE: {
				MyStorage::Accessor::Delete(key.c_str());
				res.code = 204;
			} break;
			default: {
				res.code = 500;
			} break;
		}
		
		res.end();
		return;
    });
	
	crow::logger::setLogLevel(crow::LogLevel::DEBUG);
	
	app.run();
}

