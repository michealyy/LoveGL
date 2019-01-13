#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "common.h"
#include "shader.h"

namespace kd {

struct TextureUnit{
	unsigned Index;
	std::string Name;
};

class Material {
public:
	explicit Material(const std::string& name);
	virtual ~Material();

	void Bind();
	void LoadFormFile(const std::string& path);
	void SetFloat(const char* name, float value);
	void SetInt(const char* name, int value);
	void SetVector3(const char* name, glm::vec3 vec3);
	void SetMatrix(const char* name, glm::mat4 matrix);

	void SetShader(const std::string& shader_name);
	inline void SetTexture(unsigned index, const std::string& texture_name) { textures_.push_back(TextureUnit{ index, texture_name }); }
	inline std::string GetName() { return name_; }
	inline unsigned GetID() { return current_id_; }
	bool operator==(const Material& other);
	
private:
	static unsigned MAX_ID;
	unsigned current_id_ = 0;
	std::string name_;
	std::string shader_name_;
	std::vector<TextureUnit> textures_;
	Shader* shader_;
	
	DISALLOW_COPY_AND_ASSIGN(Material)
};

}