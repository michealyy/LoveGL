#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include "shader.h"

using namespace std;
using namespace glm;
using namespace kd;

Shader::Shader(const string &name, const string &path)
{
	this->name_ = name;
	this->path_ = path;

	ifstream file;
	stringstream stream;
	file.open(path);
	if (!file.good())
	{
		string msg = "Shader file not find:";
		msg.append(path);
		fprintf(stderr, msg.c_str());
		return;
	}

	stream << file.rdbuf();
	file.close();
	string shader = stream.str();
	
	auto vertexIndex = shader.find("#vertex");
	auto fragmentIndex = shader.find("#fragment");
	auto vertexStr = shader.substr(vertexIndex + 8, fragmentIndex - vertexIndex - 8);
	auto fragmentStr = shader.substr(fragmentIndex + 10);

	unsigned vertexShader = CreateGLShader(GL_VERTEX_SHADER, vertexStr.c_str());
	unsigned fragmentShader = CreateGLShader(GL_FRAGMENT_SHADER, fragmentStr.c_str());

	program_ = glCreateProgram();
	glAttachShader(program_, vertexShader);
	glAttachShader(program_, fragmentShader);
	glLinkProgram(program_);

	int status;
	glGetProgramiv(program_, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		GLchar infoLog[1024];
		glGetProgramInfoLog(program_, 1024, NULL, infoLog);
		fprintf(stderr, infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(program_);
}

unsigned Shader::CreateGLShader(int type, const char *source)
{
	unsigned shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		fprintf(stderr, infoLog);
	}

	return shader;
}

void Shader::Bind()
{
	glUseProgram(program_);
}

void Shader::SetFloat(const char *name, float value)
{
	auto location = glGetUniformLocation(program_, name);
	if (location != -1)
		glUniform1f(location, value);
}

void Shader::SetInt(const char *name, int value)
{
	auto location = glGetUniformLocation(program_, name);
	if (location != -1)
		glUniform1i(location, value);
}

void Shader::SetVector3(const char *name, vec3 vec3)
{
	auto location = glGetUniformLocation(program_, name);
	if (location != -1)
		glUniform3f(location, vec3.x, vec3.y, vec3.z);
}

void Shader::SetVector4(const char *name, vec4 vec4)
{
	auto location = glGetUniformLocation(program_, name);
	if (location != -1)
		glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::SetMatrix(const char *name, mat4 matrix)
{
	auto location = glGetUniformLocation(program_, name);
	if (location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}