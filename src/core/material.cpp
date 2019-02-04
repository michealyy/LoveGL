#include <glad/glad.h>
#include <resource_manager.h>
#include "material.h"

using namespace std;
using namespace glm;
using namespace kd;

unsigned Material::MAX_ID = 1;

Material::Material(const string &name)
{
	auto mat = ResourceManager::GetInstance()->GetMaterial(name);
	if (mat != nullptr)
	{
		fprintf(stderr, "[Material] have same name material: %s\n", name.c_str());
		return;
	}
	name_ = name;
	current_id_ = MAX_ID;
	MAX_ID++;
	ResourceManager::GetInstance()->AddMaterial(name, this);
}

Material::~Material()
{
}

void Material::Bind()
{
	if (shader_ != nullptr)
	{
		shader_->Bind();
		TransferUniformsToShader();
	}
	else
	{
		fprintf(stderr, "[Material] bind nil shader");
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < textures_.size(); i++)
	{
		auto texture_unit = textures_[i];
		glActiveTexture(GL_TEXTURE0 + texture_unit.index);
		auto texture = ResourceManager::GetInstance()->GetTexture(texture_unit.name);
		if (texture != nullptr)
		{
			texture->Bind();
		}
		else
		{
			fprintf(stderr, "[Material] can not find texture by name: %s\n", texture_unit.name.c_str());
		}
	}
}

void Material::LoadFormFile(const string &path)
{
	//TODO: 定义材质格式
}

void Material::SetShader(const string &shader_name)
{
	auto shader = ResourceManager::GetInstance()->GetShader(shader_name);
	if (shader == nullptr)
	{
		fprintf(stderr, "[Material] can not find shader by name: %s\n", shader_name.c_str());
	}
	else
	{
		shader_ = shader;
		shader_name_ = shader_name;
	}

	//外部没有设置color,alpha时候其他材质公用的shader会影响到自己
	uniforms_["color"] = ShaderUniform{"vector3", vec3(1.f, 1.f, 1.f)};
	uniforms_["alpha"] = ShaderUniform{"float", 1.f};
}

void Material::SetTexture(const std::string &texture_name, int index, const char *uniform_name)
{
	textures_.push_back(TextureUnit{index, texture_name});
	if (uniform_name)
		uniforms_[uniform_name] = ShaderUniform{"int", index};
}

void Material::SetInt(const char *name, int value)
{
	uniforms_[name] = ShaderUniform{"int", value};
}

void Material::SetFloat(const char *name, float value)
{
	uniforms_[name] = ShaderUniform{"float", value};
}

void Material::SetVector3(const char *name, vec3 vec3)
{
	uniforms_[name] = ShaderUniform{"vector3", vec3};
}

void Material::SetVector4(const char *name, vec4 vec4)
{
	uniforms_[name] = ShaderUniform{"vector4", vec4};
}

void Material::SetColor(vec3 color)
{
	uniforms_["color"] = ShaderUniform{"vector3", color};
}

void Material::SetAlpha(float alpha)
{
	if (alpha < 1.0f)
		isTransparent = true;
	else
		isTransparent = false;

	uniforms_["alpha"] = ShaderUniform{"float", alpha};
}

void Material::TransferUniformsToShader()
{
	for (auto iter = uniforms_.begin(); iter != uniforms_.end(); iter++)
	{
		if (iter->second.type == "int")
		{
			shader_->SetInt(iter->first.c_str(), any_cast<int>(iter->second.value));
		}
		else if (iter->second.type == "float")
		{
			shader_->SetFloat(iter->first.c_str(), any_cast<float>(iter->second.value));
		}
		else if (iter->second.type == "vector3")
		{
			shader_->SetVector3(iter->first.c_str(), any_cast<vec3>(iter->second.value));
		}
		else if (iter->second.type == "vector4")
		{
			shader_->SetVector4(iter->first.c_str(), any_cast<vec4>(iter->second.value));
		}
	}
}

bool Material::operator==(const Material &other)
{
	return current_id_ == other.current_id_;
}