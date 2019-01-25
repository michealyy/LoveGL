#pragma once

#include <vector>
#include "common.h"
#include "entity.h"

namespace kd
{

class Scene
{
public:
	explicit Scene();
	virtual ~Scene();

	virtual void Setup();
	virtual void Update(float deltaTime);

	inline void AddEntity(Entity *entity)
	{
		entities_.push_back(entity);
		entity->scene = this;
	}

protected:
	std::vector<Entity *> entities_;

private:
	DISALLOW_COPY_AND_ASSIGN(Scene)
};

} // namespace kd