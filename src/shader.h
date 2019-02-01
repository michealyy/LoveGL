#pragma once

#include <any>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include "common.h"

namespace kd
{

class Shader
{
  public:
	explicit Shader(const std::string &name, const std::string &path);
	virtual ~Shader();

	void Bind();
	void SetFloat(const char *name, float value);
	void SetInt(const char *name, int value);
	void SetVector3(const char *name, glm::vec3 vec3);
	void SetVector4(const char *name, glm::vec4 vec4);
	void SetMatrix(const char *name, glm::mat4 matrix);

	inline unsigned GetGLProgramId() { return program_; }
	inline std::string GetName() { return name_; }

  private:
	unsigned program_ = 0;
	std::string path_;
	std::string name_;

	std::map<std::string, std::any> uniforms_;

	unsigned CreateGLShader(int type, const char *source);
	DISALLOW_COPY_AND_ASSIGN(Shader)
};

} // namespace kd
