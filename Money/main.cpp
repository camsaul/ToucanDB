//
//  main.cpp
//  Money
//
//  Created by Cam Saul on 8/23/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "crow.h"
#define USE_C_STRS 0
#define USE_NATIVE_STRS 1
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
//			cout << hex << &storage_ << dec << " SET: " << key << " -> " << val << endl;
			
			Storage::accessor a;
			storage_.insert(a, key);
			a->second = val;
			a.release();
		}
		
		static inline StringType Get(StringType key, bool* found) {
			Storage::const_accessor a_;
//			cout << hex << &storage_ << dec << " GET: " << key << endl;
			
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
    ([](const crow::request& req, crow::response& res, const istring& key){
		switch (req.method) {
			case crow::HTTPMethod::GET: {
				bool found;
				res.body_ = MyStorage::Accessor::Get(key, &found);
				if (!found) res.code = 404;
				res.end();
				return;
			} break;
			case crow::HTTPMethod::PUT:
			case crow::HTTPMethod::POST: {
				MyStorage::Accessor::Set(key.c_str(), req.body.c_str());
				res.code = 201;
				res.end(istring::literal("ok"));
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
	
	crow::logger::setLogLevel(crow::LogLevel::WARNING);
	
	app.run();
}

