#pragma once

#include "common.h"

namespace kd
{

class ResourceManager final : public Singleton<ResourceManager>
{
public:
	explicit ResourceManager();
	virtual ~ResourceManager();

	//texture
	//shader
	//mesh
	
	DISALLOW_COPY_AND_ASSIGN(ResourceManager)
};

} // namespace kd