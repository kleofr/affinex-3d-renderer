#include "IndexBuffer.h"

namespace AffineX
{
	void IndexBuffer::Create()
	{
		glGenBuffers(1, &m_IBO);
	}

	void IndexBuffer::Create(unsigned int IBO)
	{
		m_IBO = IBO;
		glGenBuffers(1, &m_IBO);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_IBO);
	}
	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	}
	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}