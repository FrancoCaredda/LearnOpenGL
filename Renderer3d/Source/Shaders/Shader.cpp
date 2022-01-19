#include "Shader.h"
#include "Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>

#define BUFFER_SIZE 256

Shader::Shader(GLenum type)
	: m_Type(type)
{
	m_Id = glCreateShader(m_Type);

	if (m_Id == 0)
		std::cerr << "[ERROR] " << __LINE__ << ": shader isn\'t created!" << std::endl;
}

void Shader::Load(const std::string& fileName)
{
	std::ifstream file(fileName);

	if (!file.is_open())
		throw std::exception("file wasn\'t opened!");

	char Buffer[BUFFER_SIZE] = { 0 };

	std::string source;

	while (file.good())
	{
		file.getline(Buffer, BUFFER_SIZE);

		source += Buffer;
		source += "\n";

		if (file.eof() || file.bad())
			break;
	}

	const char* sourceString = source.c_str();

	glShaderSource(m_Id, 1, &sourceString, nullptr);
}

bool Shader::Compile()
{
	glCompileShader(m_Id);

	int result;
	glGetShaderiv(m_Id, GL_COMPILE_STATUS, &result);

	char Buffer[BUFFER_SIZE] = { 0 };

	if (result == false)
	{
		glGetShaderInfoLog(m_Id, BUFFER_SIZE, nullptr, Buffer);
		std::cerr << Buffer << std::endl;

		return false;
	}

	return true;
}

Shader::~Shader()
{
	glDeleteShader(m_Id);
}

ShaderProgram::ShaderProgram()
{
	m_Id = glCreateProgram();

	if (m_Id == 0)
		std::cerr << "[ERROR] " << __LINE__ << ": shader isn\'t created!";
}

void ShaderProgram::SetVector3f(const std::string& name, const glm::vec3& vector)
{
	if (m_Uniforms.find(name) == m_Uniforms.end())
	{
		int uniform = glGetUniformLocation(m_Id, name.c_str());
		m_Uniforms[name] = uniform;
	}

	glUniform3fv(m_Uniforms[name], 1, glm::value_ptr(vector));
}

void ShaderProgram::SetVector4f(const std::string& name, const glm::vec4& vector)
{
	if (m_Uniforms.find(name) == m_Uniforms.end())
	{
		int uniform = glGetUniformLocation(m_Id, name.c_str());
		m_Uniforms[name] = uniform;
	}

	glUniform4fv(m_Uniforms[name], 1, glm::value_ptr(vector));
}

void ShaderProgram::SetMat4f(const std::string& name, const glm::mat4& matrix)
{
	if (m_Uniforms.find(name) == m_Uniforms.end())
	{
		int uniform = glGetUniformLocation(m_Id, name.c_str());
		m_Uniforms[name] = uniform;
	}

	glUniformMatrix4fv(m_Uniforms[name], 1, false, glm::value_ptr(matrix));
}

void ShaderProgram::SetFloat(const std::string& name, float value)
{
	if (m_Uniforms.find(name) == m_Uniforms.end())
	{
		int uniform = glGetUniformLocation(m_Id, name.c_str());
		m_Uniforms[name] = uniform;
	}

	glUniform1f(m_Uniforms[name], value);
}

void ShaderProgram::SetInt(const std::string& name, int value)
{
	if (m_Uniforms.find(name) == m_Uniforms.end())
	{
		int uniform = glGetUniformLocation(m_Id, name.c_str());
		m_Uniforms[name] = uniform;
	}

	glUniform1i(m_Uniforms[name], value);
}

void ShaderProgram::Attach(const Shader& shader)
{
	glAttachShader(m_Id, shader.m_Id);
}

bool ShaderProgram::Link()
{
	glLinkProgram(m_Id);

	int status;
	glGetProgramiv(m_Id, GL_LINK_STATUS, &status);

	char Buffer[BUFFER_SIZE] = { 0 };

	if (status == false)
	{
		glGetProgramInfoLog(m_Id, BUFFER_SIZE, nullptr, Buffer);
		std::cerr << Buffer << std::endl;

		return false;
	}

	glValidateProgram(m_Id);
	glGetProgramiv(m_Id, GL_VALIDATE_STATUS, &status);

	if (status == false)
	{
		glGetProgramInfoLog(m_Id, BUFFER_SIZE, nullptr, Buffer);
		std::cerr << Buffer << std::endl;

		return false;
	}

	return true;
}

void ShaderProgram::Bind()
{
	glUseProgram(m_Id);
}

void ShaderProgram::Unbind()
{
	glUseProgram(0);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_Id);
}
