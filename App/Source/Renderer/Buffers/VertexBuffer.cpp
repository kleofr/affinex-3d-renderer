#include "VertexBuffer.h"

namespace AffineX
{
	VertexBuffer::VertexBuffer(unsigned int id)
	{
		setRendererID(id);
		glGenBuffers(1, &m_RendererID);
	}
	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}
	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBuffer::addData(auto data, uint32_t size) const
	{
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
}
