#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(GLenum usage)
	: m_Usage(usage)
{
	glCreateBuffers(1, &m_Id);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Allocate(size_t size)
{
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, m_Usage);
}

void VertexBuffer::Allocate(const std::vector<float>& data)
{
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), m_Usage);
}

void VertexBuffer::Fill(const std::vector<float>& data)
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float), data.data());
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_Id);
}
