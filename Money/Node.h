//
//  Node.h
//  Money
//
//  Created by Cam Saul on 8/23/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

#include <cassert>
#include <atomic>
#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <unordered_map>

struct NodeHandle;
struct Node;
struct RootNode;

using SharedCStrPtr = std::shared_ptr<const char *>;

union NodeVal {
	SharedCStrPtr* strPtr_;
	Node* nodePtr_;
	int intVal_;
	double doubleVal_;
};

class Key;

struct Node {
	enum class Type : uint8_t {
		EMPTY,
		INT,
		FLOAT,
		STRING,
		NODE
	};
	
	friend class NodeHandle;
	friend class RootNode;
	
	// construct a new node
	static NodeHandle New();
	
	inline size_t RefCount() const { return refCount_; }
	
	Node::Type GetChildType(Key key);
	
	Node*			GetChildNode(Key key) const;
	SharedCStrPtr	GetChildString(Key key) const;
	double			GetChildDouble(Key key) const;
	int				GetChildInt(Key key) const;
	
	Node* InsertChildNode(Key key);
	
	void SetChild(Key key, NodeHandle node);
	void SetChild(Key key, SharedCStrPtr strPtr);
	void SetChild(Key key, int num);
	void SetChild(Key key, double num);
	void UnsetChild(Key key);
	
	Node(const Node&) = delete;
	
	friend std::ostream& operator<<(std::ostream& os, const Node& handle);
	
private:
	Node::Type nodeTypes_[40];
	NodeVal values_[40];
	
	std::atomic<size_t> refCount_ { 0 };
	
	NodeVal GetChild(Key key) const;
	void SetChild(Key key, Node::Type type, NodeVal child);
	
	Node(size_t refCount = 0);
	~Node();
};
