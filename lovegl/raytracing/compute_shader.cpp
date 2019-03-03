#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include "compute_shader.h"

using namespace std;
using namespace glm;
using namespace kd;

ComputeShader::ComputeShader(const string &name, const string &path)
{
    this->name_ = name;
    this->path_ = path;

    ifstream file;
    stringstream stream;
    file.open(path);
    if (!file.good())
    {
        string msg = "ComputeShader file not find:";
        msg.append(path);
        fprintf(stderr, msg.c_str());
        return;
    }

    stream << file.rdbuf();
    file.close();
    string shaderStr = stream.str();

    unsigned shader = CreateGLShader(GL_COMPUTE_SHADER, shaderStr.c_str());
    program_ = glCreateProgram();
    glAttachShader(program_, shader);
    //attach more shader
    //for shaders
    glLinkProgram(program_);

    int status;
    glGetProgramiv(program_, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        GLchar infoLog[1024];
        glGetProgramInfoLog(program_, 1024, NULL, infoLog);
        fprintf(stderr, infoLog);
    }

    glDeleteShader(shader);
}

ComputeShader::~ComputeShader()
{
    glDeleteProgram(program_);
}

unsigned ComputeShader::CreateGLShader(int type, const char *source)
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

void ComputeShader::Bind()
{
    glUseProgram(program_);
}

void ComputeShader::SetFloat(const char *name, float value)
{
    auto location = glGetUniformLocation(program_, name);
    if (location != -1)
        glUniform1f(location, value);
}

void ComputeShader::SetInt(const char *name, int value)
{
    auto location = glGetUniformLocation(program_, name);
    if (location != -1)
        glUniform1i(location, value);
}

void ComputeShader::SetVector3(const char *name, vec3 vec3)
{
    auto location = glGetUniformLocation(program_, name);
    if (location != -1)
        glUniform3f(location, vec3.x, vec3.y, vec3.z);
}

void ComputeShader::SetVector4(const char *name, vec4 vec4)
{
    auto location = glGetUniformLocation(program_, name);
    if (location != -1)
        glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);
}

void ComputeShader::SetMatrix(const char *name, mat4 matrix)
{
    auto location = glGetUniformLocation(program_, name);
    if (location != -1)
        glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

unsigned ComputeShader::GetImageUnit(const char *name)
{
    int imageBinding = 0;
    int location = glGetUniformLocation(program_, name);
    if (location != -1)
        glGetUniformiv(program_, location, &imageBinding);
    return imageBinding;
}