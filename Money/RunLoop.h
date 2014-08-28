//
//  RunLoop.h
//  Money
//
//  Created by Cam Saul on 8/24/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

class Command {
public:
	static void Get(const char* key);
	static void Set(const char* key, const char *val);
	static void SetInt(const char* key, int64_t val);
	static void SetDouble(const char* key, double val);
	static void SetString(const char* key, const char* val);
	static void Delete(const char* key);
};

enum class CommandEnum {
	/* command [key] */
	GET,
	DELETE,
	/* command [key] [val] */
	SET,
	SETINT,
	SETDOUBLE,
	SETSTRING,
};

class Parse {
public:
	static int64_t IntVal(const char* str);
	static double DoubleVal(const char* str);
	static void StringVal(char* str);
};

static const map<string, CommandEnum> kValidCommands {
	{"get",			CommandEnum::GET},
	{"set",			CommandEnum::SET},
	{"setint",		CommandEnum::SETINT},
	{"setdouble",	CommandEnum::SETDOUBLE},
	{"setstring",	CommandEnum::SETSTRING},
	{"delete",		CommandEnum::DELETE}
};

class RunLoop {
public:
	RunLoop();
private:
	char input_[256];
	char* command_;
	CommandEnum commandEnum_;
	char* key_;
	char* val_;
	void ReadAndEval();
};