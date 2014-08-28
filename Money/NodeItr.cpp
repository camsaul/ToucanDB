//
//  NodeItr.cpp
//  Money
//
//  Created by Cam Saul on 8/24/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "NodeItr.h"

NodeItr::NodeItr(const char* path):
	path_(path)
{}

inline const char* NodeItr::RemainingPath() const {
	return path_;
}

inline char NodeItr::CurrentChar() const {
	return path_[0];
}

inline char NodeItr::NextChar() const {
	return path_[1];
}

inline Node::Type NodeItr::CurrentChildType() {
	return node_->GetChildType(CurrentChar());
}

/// return true if we reached the end and it was null (e.g. can insert)
string NodeItr::GetValue() {
//	cout << "* GetValue(): '" << RemainingPath() << "'" << endl;
	
	if (!CurrentChar()) {
		return "[null] reached end of path.";
	}
	
	//		if (!NextChar()) {
	//			cout << "noNextChar. -----> [null]" << endl;
	//			return;
	//		}
	
	switch (CurrentChildType()) {
		case Node::Type::EMPTY: {
			return "[null]";
		} break;
		case Node::Type::INT: {
			auto val = node_->GetChildInt(CurrentChar());
			return "[int] " + lexical_cast<string>(val);
		} break;
		case Node::Type::FLOAT: {
			auto val = node_->GetChildDouble(CurrentChar());
			return "[float] " + lexical_cast<string>(val);
		} break;
		case Node::Type::STRING: {
			auto val = node_->GetChildString(CurrentChar());
			return string("[string] ") + *val;
		} break;
		case Node::Type::NODE: {
			if (NextChar()) {
				node_ = node_->GetChildNode(CurrentChar());
				path_++;
				return GetValue();
			}
		} break;
		default: {
			throw runtime_error("Invalid node type!");
		}
	}
	
	return "";
}

void NodeItr::CreateTo() {
	//		cout << "\n* Travel(): remaining path is '" << RemainingPath() << "'" << endl;
	if (!NextChar()) return;
	
	if (CurrentChildType() == Node::Type::NODE) {
		node_ = node_->GetChildNode(CurrentChar());
	} else {
		node_ = node_->InsertChildNode(CurrentChar());
	}
	path_++;
	if (NextChar()) CreateTo();
}

void NodeItr::SetString(const string& str) {
	CreateTo();
	node_->SetChild(CurrentChar(), str);
}