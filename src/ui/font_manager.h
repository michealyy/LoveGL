#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <stb_truetype.h>
#include "../common.h"
#include "../material.h"

namespace kd
{

struct GlyphInfo
{
	float offsetX;
	float offsetY;
	float width;
	float height;
	glm::vec3 position;
	glm::vec2 uv_right_bottom;
	glm::vec2 uv_right_top;
	glm::vec2 uv_left_top;
	glm::vec2 uv_left_bottom;
	int material_index;
};

class FontManager final : public Singleton<FontManager>
{
  public:
	explicit FontManager();
	virtual ~FontManager();

	void SetFontSize(float height);
	void Setup(const std::string &file);
	GlyphInfo GetGlyphInfo(char character, float offsetX, float offsetY);

	inline float GetFontSize() { return font_size_; }
	inline Material *GetMaterial(int index) { return materials_[index]; }

	static const unsigned TEXTURE_WIDTH = 256;
	static const unsigned TEXTURE_HEIGHT = 256;
	static const unsigned START_CHAR = 32;
	static const unsigned END_CHAR = 128;

  private:
	unsigned current_texture_count_ = 0;
	float font_size_ = 24; //24
	stbtt_bakedchar glyph_info_[END_CHAR - START_CHAR];
	std::vector<Material *> materials_;

	DISALLOW_COPY_AND_ASSIGN(FontManager)
};

} // namespace kd