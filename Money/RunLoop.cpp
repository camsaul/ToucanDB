//
//  RunLoop.cpp
//  Money
//
//  Created by Cam Saul on 8/24/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "RunLoop.h"
#include "Key.h"
#include "NodeItr.h"

void Command::Get(const char* key) {
	NodeItr(key).GetValue();
}

void Command::Set(const char* key, const char *val) {
	cout << "(Set) KEY: " << key << ", VAL: " << val << endl;
	try {
		return SetInt(key, Parse::IntVal(val));
	} catch (...) {
		try {
			return SetDouble(key, Parse::DoubleVal(val));
		} catch (...) {
			return SetString(key, val);
		}
	}
}

void Command::SetInt(const char* key, int64_t val) {
	cout << "(SetInt) KEY: " << key << ", VAL: " << val << endl;
}

void Command::SetDouble(const char* key, double val) {
	cout << "(SetDouble) KEY: " << key << ", VAL: " << val << endl;
}

void Command::SetString(const char* key, const char* val) {
	NodeItr(key).SetString(val);
}

void Command::Delete(const char* key) {
	
}

RunLoop::RunLoop() {
	while (true) {
		try {
			ReadAndEval();
		} catch (const exception& e) {
			cerr << e.what() << endl;
		}
	}
}

void RunLoop::ReadAndEval() {
start:
	cout << "$$$> ";
	std::cin.getline(input_, 256);
	command_ = strtok(input_, " ");
	
	if (!command_) {
		goto start;
	}
	Parse::StringVal(command_);
	
	const auto itr = kValidCommands.find(command_);
	if (itr == kValidCommands.end()) {
		cerr << "Invalid command: '" << command_ << '\'' << endl;
		cerr << "Valid commands: ";
		for (auto pair : kValidCommands) {
			cerr << pair.first << " ";
		}
		cerr << endl;
		goto start;
	}
	commandEnum_ = itr->second;
	
	// arg 1
	key_ = strtok(NULL, " ");
	if (!key_) {
		cerr << "Missing first arg 'key'." << endl; // TODO print usage
		goto start;
	}
	Parse::StringVal(key_);
	
	val_ = strtok(NULL, " ");
	
	if (commandEnum_ == CommandEnum::GET) {
		if (val_) throw runtime_error("Too many args to 'get'");
		return Command::Get(key_);
	} else if (commandEnum_ == CommandEnum::DELETE) {
	if (val_) throw runtime_error("Too many args to 'delete'");
		return Command::Delete(key_);
	}
	
	// arg 2
	if (!val_) {
		cerr << "Missing second arg 'value'." << endl;
		goto start;
	}
	
	// make sure there's no third arg
	char* extra = strtok(NULL, " ");
	if (extra) {
		cerr << "Extra arg: " << extra << endl;
		goto start;
	}
	
	switch (commandEnum_) {
		case CommandEnum::SET:			return Command::Set		 (key_, val_);
		case CommandEnum::SETINT:		return Command::SetInt	 (key_, Parse::IntVal(val_));
		case CommandEnum::SETDOUBLE:	return Command::SetDouble(key_, Parse::DoubleVal(val_));
		case CommandEnum::SETSTRING:	{
			Parse::StringVal(val_);
			return Command::SetString(key_, val_);
		}
		default: break; // can't reach here
	}
}

int64_t Parse::IntVal(const char* str) {
	char* endptr = nullptr;
	int64_t result = strtol(str, &endptr, 0);
	if (endptr) {
		throw runtime_error(string("Invalid int: ") + string(endptr));
	}
	return result;
}

double Parse::DoubleVal(const char* str) {
	char* endptr = nullptr;
	double result = strtod(str, &endptr);
	if (endptr) {
		throw runtime_error(string("Invalid float: ") + string(endptr));
	}
	return result;
}

void Parse::StringVal(char* str) {
	size_t len = strlen(str);
	for (int i = 0; i < len; i++) {
		str[i] = std::tolower(str[i]);
		if (!Key::IsValid(str[i])) {
			throw runtime_error(string("Invalid char: '") + lexical_cast<string>(str[i]) + "");
		}
	}
}
