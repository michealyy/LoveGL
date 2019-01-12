#pragma once

#include <vector>
#include "common.h"

namespace kd
{

class Scene
{
  public:
	explicit Scene();
	virtual ~Scene();
	virtual void OnSetup();
	virtual void OnUpdate();

  private:
	DISALLOW_COPY_AND_ASSIGN(Scene)
};

} // namespace kd