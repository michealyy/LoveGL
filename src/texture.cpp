#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Texture.h"
#include "engine.h"

using namespace std;
using namespace kd;

Texture::Texture(const std::string &name)
{
	auto tex = Engine::GetInstance()->GetTexture(name);
	if (tex != nullptr)
	{
		fprintf(stderr, "[Texture] have same name texture: %s\n", name.c_str());
		return;
	}
	name_ = name;
	Engine::GetInstance()->AddTexture(name, this);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture_id_);
}

void Texture::Bind()
{
	if (texture_id_ == 0)
	{
		fprintf(stderr, "[Texture] opengl texture id error: %s\n", name_.c_str());
	}
	glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void Texture::LoadFormFile(const std::string &path)
{
	stbi_set_flip_vertically_on_load(true);

	unsigned texture(0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (data)
	{
		this->width_ = width;
		this->height_ = height;
		this->texture_id_ = texture;

		if (channels > 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
		}
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		fprintf(stderr, "[Texture] Texture File Not Find or Not Support: %s\n", path.c_str());
	}
	stbi_image_free(data);
}
