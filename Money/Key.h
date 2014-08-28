//
//  Key.h
//  Money
//
//  Created by Cam Saul on 8/24/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

class Key {
public:
	inline Key(char c):
	value_(kCharToIntMap[c])
	{
		if (!value_) { // ASCII only
			cout << "Invalid char: '" << c << "' ((int)c = " << (int)c << ")" << endl;
			string message {"Invalid character: "};
			message += c;
			throw invalid_argument(message);
		}
	}
	
	static inline bool IsValid(char c) {
		return kCharToIntMap[c] != '\0';
	}
	
	inline Key(int i):
	value_(i + 1)
	{}
	
	inline operator u_int8_t() const {
		return value_ - 1;
	}
	
	inline explicit operator char() const {
		assert(value_ > 0 && value_ <= 40);
		return kIntToCharMap[value_ - 1];
	}
	
	static void Init() {
		kCharToIntMap = (int*)calloc(sizeof(int), 256);
		kIntToCharMap = (char*)malloc(sizeof(char) * 40);
		for (int i = 0; i < 40; i++) {
			char c = kValidChars[i];
			kCharToIntMap[(int)c] = i + 1; // add one to distinguish from 0's
			kIntToCharMap[i] = c;
		}
	}
private:
	u_int8_t value_;
	
	static const char * const kValidChars;
	static int* kCharToIntMap;
	static char* kIntToCharMap;
};