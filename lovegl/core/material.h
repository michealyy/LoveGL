#pragma once

#include <string>
#include <vector>
#include <map>
#include <any>
#include <glm/glm.hpp>
#include "common.h"
#include "shader.h"

namespace kd
{

struct TextureUnit
{
	int index;
	std::string name;
};

struct ShaderUniform
{
	std::string type;
	std::any value;
};

//TODO: 多pass支持
/*
*	PBR材质贴图
*	0 diffuseCubemap
*	1 specularCubemap
*	2 brdfLUT
*	3 albedo
*	4 metallicRoughnessTexture
*/
class Material
{
  public:
	explicit Material(const std::string &name);
	virtual ~Material();

	void Bind();
	void LoadFormFile(const std::string &path);

	void SetInt(const char *name, int value);
	void SetFloat(const char *name, float value);
	void SetVector3(const char *name, glm::vec3 vec3);
	void SetVector4(const char *name, glm::vec4 vec4);
	//通用2个Uniform，color+alpha
	void SetColor(glm::vec3 color);
	void SetAlpha(float alpha);
	void SetShader(const std::string &shader_name);
	void SetTexture(const std::string &texture_name, int index = 0, const char *uniform_name = nullptr);

	//void SetAlbedo()
	//void SetMetallic

	inline std::string GetName() { return name_; }
	inline Shader *GetShader() { return shader_; }
	inline unsigned GetID() { return current_id_; }
	bool operator==(const Material &other);

	bool isTransparent = false;

  private:
	void TransferUniformsToShader();
	static unsigned MAX_ID;
	unsigned current_id_ = 0;
	std::string name_;
	std::string shader_name_;
	std::vector<TextureUnit> textures_;
	std::map<std::string, ShaderUniform> uniforms_;
	Shader *shader_;

	DISALLOW_COPY_AND_ASSIGN(Material)
};

} // namespace kd
