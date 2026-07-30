#include "VertexBuffer.h"

namespace AffineX
{
	void VertexBuffer::Create()
	{
		glGenBuffers(1, &m_VBO);
	}

	void VertexBuffer::Create(unsigned int VBO)
	{
		m_VBO = VBO;
		glGenBuffers(1, &m_VBO);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_VBO);
	}
	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}
	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
