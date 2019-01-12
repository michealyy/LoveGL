#pragma once

#include <string>
#include <vector>
#include "common.h"

namespace kd {
	class Node {
	public:
		explicit Node();
		virtual ~Node();

		void Setup();
		void Update();
		virtual void OnSetup();
		virtual void OnUpdate();
		
		virtual void AddChild(Node * child);
		Node * FindChild(const std::string& name);
		void RemoveChild(const std::string& name);
		void RemoveAllChild();

		inline std::string GetName() { return name_; }
	protected:
		std::vector<Node*> children_;
		std::string name_;
		Node* parent_;
		bool dirty_ = true;
	private:
		DISALLOW_COPY_AND_ASSIGN(Node)
	};
}