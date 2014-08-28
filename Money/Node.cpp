//
//  Node.cpp
//  Money
//
//  Created by Cam Saul on 8/24/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Node.h"
#include "NodeHandle.h"

#pragma mark - Node

static NodeHandle sRootNode = Node::New();

Node::Node(size_t refCount):
refCount_ { refCount }
{
	memset((void*)nodeTypes_, 0, sizeof(nodeTypes_));
}

Node::~Node() {
//	cout << "~" << *this << endl;
	assert(refCount_ == 0);
	for (int i = 0; i < 40; i++) {
		UnsetChild(i); // todo - UnsetChild also sets key to empty, we don't need to do that
	}
}

NodeHandle& Node::GetRoot() {
	return sRootNode;
}

Node::Type Node::GetChildType(Key key) {
	return nodeTypes_[key];
}

NodeVal Node::GetChild(Key key) const {
	return values_[key];
}

Node* Node::GetChildNode(Key key) const {
//	cout << *this << " GetChildNode " << (char)key << endl;
	return GetChild(key).nodePtr_;
}

SharedCStrPtr Node::GetChildString(Key key) const {
//	cout << *this << " GetChildString " << (char)key << endl;
	return *GetChild(key).strPtr_;
}

double Node::GetChildDouble(Key key) const {
	return GetChild(key).doubleVal_;
}

int64_t Node::GetChildInt(Key key) const {
	return GetChild(key).intVal_;
}

void Node::SetChild(Key key, Node::Type type, NodeVal child) {
//	cout << *this << ": setting child node: '" << (char)key << '\'' << endl;
	// unset old child, if applicable
	UnsetChild(key);
	
	if (type == Type::EMPTY) return;
	
	nodeTypes_[key]= type;
	values_[key] = child;
}

Node* Node::InsertChildNode(Key key) {
	NodeVal val;
	Node* child = new Node(1);
	val.nodePtr_ = child;
	SetChild(key, Type::NODE, val);
	return child;
}

void Node::SetChild(Key key, NodeHandle node) {
	if (!node) {
		UnsetChild(key);
		return;
	}
	
	NodeVal val;
	val.nodePtr_ = &(*node);
	assert((int)node->refCount_ >= 0);
	val.nodePtr_->refCount_++;
	SetChild(key, Type::NODE, val);
}

void Node::SetChild(Key key, const string& str) {
	NodeVal val;
	cout << "STR: " << str << endl;
	val.strPtr_ = new SharedCStrPtr;
	*val.strPtr_ = make_shared<const char *>(str.c_str());
	SetChild(key, Type::STRING, val);
	
	cout << "VAL: " << *GetChildString(key) << endl;
}

void Node::SetChild(Key key, int64_t num) {
	NodeVal val;
	val.intVal_ = num;
	SetChild(key, Type::INT, val);
}

void Node::SetChild(Key key, double num) {
	NodeVal val;
	val.doubleVal_ = num;
	SetChild(key, Type::FLOAT, val);
}

void Node::UnsetChild(Key key) {
	switch (GetChildType(key)) {
		case Type::EMPTY: return;
			nodeTypes_[key] = Type::EMPTY;
			
		case Type::INT:
		case Type::FLOAT: {
			return;
		}
			
		case Type::STRING: {
			auto str = values_[key].strPtr_;
			delete str;
		} break;
		case Type::NODE: {
			auto node = values_[key].nodePtr_;
			assert(node->refCount_ > 0);
			node->refCount_--;
			if (node->refCount_ == 0) {
				delete node;
			}
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Node& node) {
	os << "Node: 0x" << hex << (size_t)&node % 65536 << dec << " [rc = " << (size_t)node.refCount_ << "]";
	return os;
}