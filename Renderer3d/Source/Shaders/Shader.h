#pragma once

#include "GL/glew.h"

#include <glm/glm.hpp>

#include <cstdint>
#include <string>
#include <unordered_map>

class Shader
{
public:
	Shader(GLenum type);
	Shader(const Shader&) = delete;
	
	void Load(const std::string& fileName);
	bool Compile();

	~Shader();

	friend class ShaderProgram;
private:
	uint32_t m_Id;
	GLenum m_Type;
};

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const ShaderProgram&) = delete;

	void SetVector3f(const std::string& name, const glm::vec3& vector);
	void SetMat4f(const std::string& name, const glm::mat4& matrix);
	void SetFloat(const std::string& name, float value);
	void SetImage(const std::string& name, int level);

	void Attach(const Shader& shader);
	bool Link();

	void Bind();
	void Unbind();

	~ShaderProgram();
private:
	uint32_t m_Id;
	std::unordered_map<std::string, int> m_Uniforms;
};