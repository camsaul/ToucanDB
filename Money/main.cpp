//
//  main.cpp
//  Money
//
//  Created by Cam Saul on 8/23/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Node.h"
#include "NodeHandle.h"

using namespace std;

#pragma mark - Keys

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

const char * const Key::kValidChars = "+./_0123456789abcdefghijklmnopqrstuvwxyz";
int* Key::kCharToIntMap = nullptr;
char* Key::kIntToCharMap = nullptr;

#pragma mark - Node

Node::Node(size_t refCount):
	refCount_ { refCount }
{
	memset((void*)nodeTypes_, 0, sizeof(nodeTypes_));
}

Node::~Node() {
	cout << "~" << *this << endl;
	assert(refCount_ == 0);
	for (int i = 0; i < 40; i++) {
		UnsetChild(i); // todo - UnsetChild also sets key to empty, we don't need to do that
	}
}

Node::Type Node::GetChildType(Key key) {
	return nodeTypes_[key];
}

NodeVal Node::GetChild(Key key) const {
	return values_[key];
}

Node* Node::GetChildNode(Key key) const {
	cout << *this << " GetChildNode " << (char)key << endl;
	return GetChild(key).nodePtr_;
}

SharedCStrPtr Node::GetChildString(Key key) const {
	cout << *this << " GetChildString " << (char)key << endl;
	return *GetChild(key).strPtr_;
}

double Node::GetChildDouble(Key key) const {
	return GetChild(key).doubleVal_;
}

int Node::GetChildInt(Key key) const {
	return GetChild(key).intVal_;
}

void Node::SetChild(Key key, Node::Type type, NodeVal child) {
	cout << *this << ": setting child node: '" << (char)key << '\'' << endl;
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

void Node::SetChild(Key key, SharedCStrPtr strPtr) {
	NodeVal val;
	val.strPtr_ = new SharedCStrPtr;
	*val.strPtr_ = strPtr;
	SetChild(key, Type::STRING, val);
}

void Node::SetChild(Key key, int num) {
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

#pragma mark - Other Stuff

static NodeHandle sRootNode = Node::New();

class NodeItr {
public:
	NodeHandle node_ = sRootNode.Copy();
	const char * path_ = nullptr;
	NodeItr(const char* path):
		path_(path)
	{}
	
	~NodeItr() {
//		cout << "~NodeItr():" << node_ << endl;
	}
	
	inline const char *RemainingPath() const {
		return path_;
	}
	
	inline char CurrentChar() const {
		return path_[0];
	}
	
	inline char NextChar() const {
		return path_[1];
	}
	
	inline Node::Type CurrentChildType() {
		return node_->GetChildType(CurrentChar());
	}
	
	/// return true if we reached the end and it was null (e.g. can insert)
	void GetValue() {
		cout << "* GetValue(): '" << RemainingPath() << "'" << endl;
		
		if (!CurrentChar()) {
			cout << "[null] reached end of path." << endl;
			return;
		}

//		if (!NextChar()) {
//			cout << "noNextChar. -----> [null]" << endl;
//			return;
//		}
		
		switch (CurrentChildType()) {
			case Node::Type::EMPTY: {
				cout << "node is empty -----> [null]" << endl;
			} break;
			case Node::Type::INT: {
				auto val = node_->GetChildInt(CurrentChar());
				cout << "-----> [int] " << val << endl;
			} break;
			case Node::Type::FLOAT: {
				auto val = node_->GetChildDouble(CurrentChar());
				cout << "-----> [float] " << val << endl;
			} break;
			case Node::Type::STRING: {
				auto val = node_->GetChildString(CurrentChar());
				cout << "-----> [string] " << *val << endl;
			} break;
			case Node::Type::NODE: {
				if (NextChar()) {
					node_ = node_->GetChildNode(CurrentChar());
					path_++;
					GetValue();
				}
			} break;
			default: {
				throw runtime_error("Invalid node type!");
			}
		}
	}
	
	/// travel to one step before end
	void CreateTo() {
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
	
	void SetString(SharedCStrPtr str) {
		CreateTo();
		node_->SetChild(CurrentChar(), str);
	}
};

int main(int argc, const char * argv[])
{
	Key::Init();
	
	assert(sRootNode->RefCount() == 1);
	cout << "Root node is: " << *sRootNode << endl;
	
	NodeItr("cam").SetString(make_shared<const char *>("This is cam's cool string!"));
	NodeItr("cam").GetValue();
	
	NodeItr("cam_burger").SetString(make_shared<const char *>("Another cool string!"));
	NodeItr("cam").GetValue();
	NodeItr("cam_burger").GetValue();
	NodeItr("cam_b").GetValue();
	
	cout << "set cam -> HAY" << endl;
	NodeItr("cam").SetString(make_shared<const char *>("HAY"));
	NodeItr("cam").GetValue();
	NodeItr("cam_burger").GetValue();
	
	cout << "set cam_ -> Gone" << endl;
	NodeItr("cam_").SetString(make_shared<const char *>("Gone."));
	NodeItr("cam_").GetValue();
	NodeItr("cam_burger").GetValue();
	
	cout << "set a -> Gone" << endl;
	NodeItr("aaa").SetString(make_shared<const char *>("Gone."));
	NodeItr("aa").SetString(make_shared<const char *>("Gone."));
	NodeItr("a").SetString(make_shared<const char *>("Gone."));
	NodeItr("aaa").GetValue();
	
	cout << "deleting root node: " << sRootNode << endl;
	assert(sRootNode->RefCount() == 1);
	sRootNode = nullptr;
}

