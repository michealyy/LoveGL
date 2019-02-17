#pragma once

#include <string>
#include <vector>
#include "common.h"

namespace kd
{

unsigned GLILoadCreateGLTexture(const std::string &file, bool srgb = false);

class Texture
{
public:
	explicit Texture(const std::string &name);
	virtual ~Texture();

	void Bind();
	void LoadFormFile(const std::string &path);
	void LoadFormData(int width, int height, int channels, std::vector<unsigned char> &data);
	inline std::string GetName() { return name_; }
	inline void SetTextureId(unsigned texture_id) { texture_id_ = texture_id; }
	inline unsigned GetTextureId() { return texture_id_; }

private:
	std::string name_;
	int width_ = 0;
	int height_ = 0;
	unsigned int texture_id_ = 0;

	DISALLOW_COPY_AND_ASSIGN(Texture)
};

} // namespace kd
