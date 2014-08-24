//
//  NodeHandle.h
//  Money
//
//  Created by Cam Saul on 8/23/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

struct Node;

struct NodeHandle {
	NodeHandle(Node* node = nullptr);
	~NodeHandle();
		
	NodeHandle& operator=(const NodeHandle&) = delete;
	NodeHandle(NodeHandle&&);
	NodeHandle& operator=(NodeHandle&&);
	
	NodeHandle& operator=(Node*);
	
	NodeHandle Copy() const {
		return *this;
	}
	
	Node& operator*();
	Node* operator->();
	
	operator bool() const;
	
	friend std::ostream& operator<<(std::ostream& os, const NodeHandle& handle);
	
private:
	Node* node_ = nullptr;
	NodeHandle(const NodeHandle&);
};