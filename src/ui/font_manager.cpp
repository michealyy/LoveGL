#include "font_manager.h"
#include <filesystem>
#include <glad/glad.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#include "../config.h"
#include "../texture.h"

using namespace std;

namespace kd
{

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
}

void FontManager::Setup(const std::string &file)
{
	filesystem::path aPath(ASSET_PATH);
	auto path = aPath / "fonts" / file;

	/* load font file */
	long size;
	unsigned char *fontBuffer;

	FILE *fontFile = fopen(path.string().c_str(), "rb");
	fseek(fontFile, 0, SEEK_END);
	size = ftell(fontFile);		  /* how long is the file ? */
	fseek(fontFile, 0, SEEK_SET); /* reset */

	fontBuffer = (unsigned char *)malloc(size);

	fread(fontBuffer, size, 1, fontFile);
	fclose(fontFile);

	/* prepare font */
	stbtt_fontinfo info;
	if (!stbtt_InitFont(&info, fontBuffer, 0))
	{
		fprintf(stderr, "[FontManager] ttf file not find : %s\n", file.c_str());
	}

	unsigned char *bitmap = (unsigned char *)malloc(TEXTURE_WIDTH * TEXTURE_HEIGHT);
	memset(bitmap, 0, TEXTURE_WIDTH * TEXTURE_HEIGHT);

	//stb_truetype直接生成START_CHAR到END_CHAR字体图集
	stbtt_BakeFontBitmap(fontBuffer, 0, font_size_, bitmap, TEXTURE_WIDTH, TEXTURE_HEIGHT, START_CHAR, END_CHAR - START_CHAR, glyph_info_);

	//生成材质贴图
	string texture_name("font_");
	texture_name.append(std::to_string(current_texture_count_));
	auto texture = new Texture(texture_name);
	auto font_mat = new Material(texture_name);
	font_mat->SetShader("font");
	font_mat->SetTexture(0, texture_name);
	materials_.push_back(font_mat);
	current_texture_count_++;

	unsigned texture_id;
	glGenTextures(1, &texture_id);
	texture->SetTextureId(texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	//解除纹理4字节对齐限制。字体只需要用到1个通道
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//使用mipmap小字更清楚。（写在update里面）
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	glGenerateMipmap(GL_TEXTURE_2D);	
}

GlyphInfo FontManager::GetGlyphInfo(char character, float offsetX, float offsetY)
{
	stbtt_aligned_quad quad;
	float x = 0;
	float y = 0;
	stbtt_GetBakedQuad(glyph_info_, TEXTURE_WIDTH, TEXTURE_HEIGHT, character - START_CHAR, &x, &y, &quad, 1);

	auto xmin = quad.x0;
	auto xmax = quad.x1;
	auto ymin = -quad.y1;
	auto ymax = -quad.y0;

	GlyphInfo info{};
	info.width = xmax - xmin;
	info.height = ymax - ymin;
	info.position = {xmin, ymin, 0};
	info.uv_left_bottom = {quad.s0, quad.t1};
	info.uv_left_top = {quad.s0, quad.t0};
	info.uv_right_top = {quad.s1, quad.t0};
	info.uv_right_bottom = {quad.s1, quad.t1};

	return info;
}

}