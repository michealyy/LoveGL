#pragma once

#include "common.h"

namespace kd
{

class ResourceMananger final : public Singleton<ResourceMananger>
{
public:
	explicit ResourceMananger();
	virtual ~ResourceMananger();

	//texture
	//shader
	//mesh
	
	DISALLOW_COPY_AND_ASSIGN(ResourceMananger)
};

} // namespace kd