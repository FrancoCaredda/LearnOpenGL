#pragma once

#include "GL/glew.h"

#include <cstdint>
#include <vector>

class VertexBuffer
{
public:
	VertexBuffer(GLenum usage);
	VertexBuffer(const VertexBuffer&) = delete;

	void Bind();
	void Unbind();

	void Allocate(size_t size);
	void Allocate(const std::vector<float>& data);
	void Fill(const std::vector<float>& data);

	~VertexBuffer();
private:
	uint32_t m_Id = -1;
	GLenum m_Usage;
};