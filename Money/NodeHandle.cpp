//
//  NodeHandle.cpp
//  Money
//
//  Created by Cam Saul on 8/23/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include <cassert>

#include "Node.h"
#include "NodeHandle.h"

using namespace std;

NodeHandle::NodeHandle(Node* node):
	node_ { node }
{
	if (node) node_->refCount_++;
//	cout << "Created: " << *this << endl;
}
	
NodeHandle::~NodeHandle() {
	if (node_) node_->refCount_--;
//	cout << "~" << *this << endl;
	
	if (!node_) return;
		
	if (node_->refCount_ == 0) {
		delete node_;
	}
	node_ = nullptr;
}

NodeHandle::NodeHandle(const NodeHandle& rhs):
	node_ { rhs.node_ }
{
	if (this == &rhs) {
//		cout << "copy: returning because ==" << endl;
		return;
	}

	if (node_) {
		assert(node_->refCount_ > 0);
		node_->refCount_++;
	}
	
//	cout << "Copied from: " << rhs << endl;
//	cout << "         to: " << *this << endl;
}

//NodeHandle NodeHandle::operator=(const NodeHandle& rhs) {
//	if (*this == rhs) {
//		return *this;
//	}
//}

NodeHandle::NodeHandle(NodeHandle&& rhs):
	node_ { rhs.node_ }
{
//	cout << "Move-construct from: " << rhs << endl;
//	cout << "                 to: " << *this << endl;
	rhs.node_ = nullptr;
}

NodeHandle& NodeHandle::operator=(NodeHandle&& rhs) {
	if (this == &rhs) {
//		cout << "operator=: returning because ==" << endl;
		return *this;
	}
//	cout << "Move-assign from: " << rhs << endl;
//	cout << "              to: " << *this << endl;
//	
	node_->refCount_--;
	
	if (node_->refCount_ == 0) {
		delete node_;
	}
	node_ = rhs.node_;
	rhs.node_ = nullptr;
		
//	cout << "     (DONE) from: " << rhs << endl;
//	cout << "              to: " << *this << endl;
	return *this;
}

NodeHandle& NodeHandle::operator=(Node* node) {
	if (node_ == node) {
		return *this; // self-assignment
	}
	
//	cout << "Move-assign from Node*: " << *this << endl;
	
	if (node_) {
		node_->refCount_--;
		if (node_->refCount_ == 0) {
			delete node_;
		}
	}
	
	node_ = node;
	if (node_) {
		node_->refCount_++;
	}
	
//	cout << "              finished: " << *this << endl;
	
	return *this;
}
	
Node& NodeHandle::operator*() {
	if (!node_) {
		throw runtime_error { "Attempt to derefence NodeHandle to node that does not exist!" };
	}
	return *node_;
}
	
Node* NodeHandle::operator->() {
	if (!node_) {
		throw runtime_error { "Attempt to derefence NodeHandle to node that does not exist!" };
	}
	return node_;
}
	
NodeHandle::operator bool() const {
	return node_ && node_->refCount_ > 0;
}
	
NodeHandle Node::New() {
	auto node = new Node();
	assert(node->refCount_ == 0);
	auto handle = NodeHandle(node);
	assert(handle->RefCount() == 1);
	return handle;
}
	
std::ostream& operator<<(std::ostream& os, const NodeHandle& handle) {
	os << "NodeHandle: 0x" << hex << ((size_t)&handle % 65536) << dec << " -> ";
	if (handle.node_)	os << *handle.node_;
	else				os << "nullptr";
	return os;
}
