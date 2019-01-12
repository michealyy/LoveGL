#include "node.h"

using namespace std;
using namespace kd;

Node::Node() {
}

Node::~Node() {
	for (auto child : children_) {
		SafeDelete(child);
	}
}

void Node::Setup() {
	OnSetup();

	for (auto child : children_) {
		child->Setup();
	}
}

void Node::Update() {
	OnUpdate();

	for (auto child : children_) {
		child->Update();
	}
}

void Node::OnSetup() {
}

void Node::OnUpdate() {
}

void Node::AddChild(Node * child) {
	if (child == nullptr) {
		return;
	}
	child->parent_ = this;
	children_.push_back(child);

	dirty_ = true;
}

Node * Node::FindChild(const string& name) {
	for (auto child : children_) {
		if (child->GetName() == name) {
			return child;
		}
	}

	return nullptr;
}

void Node::RemoveChild(const string& name) {
}

void Node::RemoveAllChild() {
	children_.clear();
}