#pragma once

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <vector>

class VertexArray
{
public:
	VertexArray();
	VertexArray(const VertexArray&) = delete;

	void Bind();
	void Unbind();
	
	void SetVertexAttributes(const std::vector<std::pair<int, glm::vec3>>& attribs);
	void EnableVertexAttibutes();

	~VertexArray();
private:
	uint32_t m_Id;
	std::vector<int> m_AttribsId;
};

