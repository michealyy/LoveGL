#include <glad/glad.h>
#include "material.h"
#include "engine.h"

using namespace std;
using namespace glm;
using namespace kd;

unsigned Material::MAX_ID = 1;

Material::Material(const string & name) {
	auto mat = Engine::GetInstance()->GetMaterial(name);
	if (mat != nullptr) {
		fprintf(stderr, "[Material] have same name material: %s\n", name.c_str());
		return;
	}
	name_ = name;
	current_id_ = MAX_ID;
	MAX_ID++;
	Engine::GetInstance()->AddMaterial(name, this);
}

Material::~Material() {
}

void Material::Bind() {
	if (shader_ != nullptr) {
		shader_->Bind();
	}
	else {
		fprintf(stderr, "[Material] bind nil shader");
	}

	for (int i = 0; i < textures_.size(); i++) {
		auto texture_unit = textures_[i];
		glActiveTexture(GL_TEXTURE0 + texture_unit.Index);
		auto texture = Engine::GetInstance()->GetTexture(texture_unit.Name);
		if (texture != nullptr) {
			texture->Bind();
		}
		else {
			fprintf(stderr, "[Material] can not find texture by name: %s\n", texture_unit.Name.c_str());
		}
	}
}

void Material::LoadFormFile(const string & path) {
}

void Material::SetShader(const string & shader_name) {
	auto shader = Engine::GetInstance()->GetShader(shader_name);
	if (shader == nullptr) {
		fprintf(stderr, "[Material] can not find shader by name: %s\n", shader_name.c_str());
	}
	else {
		shader_ = shader;
		shader_name_ = shader_name;
	}
}

void Material::SetFloat(const char * name, float value) {
	shader_->SetFloat(name, value);
}

void Material::SetInt(const char * name, int value) {
	shader_->SetInt(name, value);
}

void Material::SetVector3(const char * name, vec3 vec3) {
	shader_->SetVector3(name, vec3);
}

void Material::SetMatrix(const char * name, mat4 matrix) {
	shader_->SetMatrix(name, matrix);
}

bool Material::operator==(const Material & other) {
	return current_id_ == other.current_id_;
}