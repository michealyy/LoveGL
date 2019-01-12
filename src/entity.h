#pragma once

#include <string>
#include <vector>
#include "common.h"

namespace kd {

class Entity {
public:
	explicit Entity();
	virtual ~Entity();

	virtual void Setup();
	virtual void Update(float deltaTime);

	virtual void AddChild(Entity * child);
	Entity * FindChild(const std::string& name);
	void RemoveChild(const std::string& name);
	void RemoveAllChild();

	inline std::string GetName() { return name_; }
protected:
	std::vector<Entity*> children_;
	std::string name_;
	Entity* parent_;
	bool dirty_ = true;
private:
	DISALLOW_COPY_AND_ASSIGN(Entity)
};

}