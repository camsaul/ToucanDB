//
//  NodeItr.h
//  Money
//
//  Created by Cam Saul on 8/24/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include "Node.h"
#include "NodeHandle.h"

class NodeItr {
public:
	NodeHandle node_ = Node::GetRoot().Copy();
	const char * path_ = nullptr;
	
	NodeItr(const char* path);
	
	const char *RemainingPath() const;
	
	char CurrentChar() const;
	
	char NextChar() const;
	
	Node::Type CurrentChildType();
	
	/// return true if we reached the end and it was null (e.g. can insert)
	void GetValue();
	
	/// travel to one step before end
	void CreateTo();
	
	void SetString(SharedCStrPtr str);
};