#include "VertexArray.h"

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_Id);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_Id);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::SetVertexAttributes(const std::vector<std::pair<int, glm::vec3>>& attribs)
{
	for (int i = 0; i < attribs.size(); i++)
		m_AttribsId.push_back(attribs[i].first);

	for (int i = 0; i < attribs.size(); i++)
	{
		glVertexAttribPointer(m_AttribsId[i], (int)attribs[i].second.x, GL_FLOAT, GL_FALSE, (int)attribs[i].second.y * sizeof(float),
			(const void*)((int)attribs[i].second.z * sizeof(float)));
	}
}

void VertexArray::EnableVertexAttibutes()
{
	for (int i = 0; i < m_AttribsId.size(); i++)
		glEnableVertexAttribArray(m_AttribsId[i]);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Id);
}
