#include <GL/glew.h>
#include <gli/gli.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "texture.h"
#include <resource_manager.h>

using namespace std;

namespace kd
{

Texture::Texture(const std::string &name)
{
	auto tex = ResourceManager::GetInstance()->GetTexture(name);
	if (tex != nullptr)
	{
		fprintf(stderr, "[Texture] have same name texture: %s\n", name.c_str());
		return;
	}
	name_ = name;
	ResourceManager::GetInstance()->AddTexture(name, this);
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
	}
	else
	{
		fprintf(stderr, "[Texture] Texture File Not Find or Not Support: %s\n", path.c_str());
	}
	stbi_image_free(data);
}

void Texture::LoadFormData(int width, int height, int channels, vector<unsigned char> &data)
{
	unsigned texture(0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->width_ = width;
	this->height_ = height;
	this->texture_id_ = texture;
	
	if (channels > 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
}

unsigned GLILoadCreateGLTexture(const string &file, bool srgb)
{
	gli::texture Texture = gli::load(file.c_str());
	if (Texture.empty())
		return 0;

	gli::gl GL(gli::gl::PROFILE_GL33);
	gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
	GLenum Target = GL.translate(Texture.target());

	GLuint TextureName = 0;
	glGenTextures(1, &TextureName);
	glBindTexture(Target, TextureName);
	glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_R, Format.Swizzles[0]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_G, Format.Swizzles[1]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_B, Format.Swizzles[2]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_A, Format.Swizzles[3]);

	glm::tvec3<GLsizei> const Extent(Texture.extent());
	GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());

	switch (Texture.target())
	{
	case gli::TARGET_1D:
		glTexStorage1D(
			Target, static_cast<GLint>(Texture.levels()), srgb ? GL_SRGB : Format.Internal, Extent.x);
		break;
	case gli::TARGET_1D_ARRAY:
	case gli::TARGET_2D:
	case gli::TARGET_CUBE:
		glTexStorage2D(
			Target, static_cast<GLint>(Texture.levels()), srgb ? GL_SRGB : Format.Internal,
			Extent.x, Texture.target() != gli::TARGET_1D_ARRAY ? Extent.y : FaceTotal);
		break;
	case gli::TARGET_2D_ARRAY:
	case gli::TARGET_3D:
	case gli::TARGET_CUBE_ARRAY:
		glTexStorage3D(
			Target, static_cast<GLint>(Texture.levels()), srgb ? GL_SRGB : Format.Internal,
			Extent.x, Extent.y,
			Texture.target() == gli::TARGET_3D ? Extent.z : FaceTotal);
		break;
	default:
		assert(0);
		break;
	}

	for (std::size_t Layer = 0; Layer < Texture.layers(); ++Layer)
	{
		for (std::size_t Face = 0; Face < Texture.faces(); ++Face)
		{
			for (std::size_t Level = 0; Level < Texture.levels(); ++Level)
			{
				GLsizei const LayerGL = static_cast<GLsizei>(Layer);
				glm::tvec3<GLsizei> Extent(Texture.extent(Level));
				Target = gli::is_target_cube(Texture.target())
							 ? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
							 : Target;

				switch (Texture.target())
				{
				case gli::TARGET_1D:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage1D(
							Target, static_cast<GLint>(Level), 0, Extent.x,
							srgb ? GL_SRGB : Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
							Texture.data(Layer, Face, Level));
					else
						glTexSubImage1D(
							Target, static_cast<GLint>(Level), 0, Extent.x,
							Format.External, Format.Type,
							Texture.data(Layer, Face, Level));
					break;
				case gli::TARGET_1D_ARRAY:
				case gli::TARGET_2D:
				case gli::TARGET_CUBE:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage2D(
							Target, static_cast<GLint>(Level),
							0, 0,
							Extent.x,
							Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
							srgb ? GL_SRGB : Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
							Texture.data(Layer, Face, Level));
					else
						glTexSubImage2D(
							Target, static_cast<GLint>(Level),
							0, 0,
							Extent.x,
							Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
							Format.External, Format.Type,
							Texture.data(Layer, Face, Level));
					break;
				case gli::TARGET_2D_ARRAY:
				case gli::TARGET_3D:
				case gli::TARGET_CUBE_ARRAY:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage3D(
							Target, static_cast<GLint>(Level),
							0, 0, 0,
							Extent.x, Extent.y,
							Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
							srgb ? GL_SRGB : Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
							Texture.data(Layer, Face, Level));
					else
						glTexSubImage3D(
							Target, static_cast<GLint>(Level),
							0, 0, 0,
							Extent.x, Extent.y,
							Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
							Format.External, Format.Type,
							Texture.data(Layer, Face, Level));
					break;
				default:
					assert(0);
					break;
				}
			}
		}
	}

	return TextureName;
}

}