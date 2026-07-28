#include "VertexArrayObject.h"

namespace AffineX
{
	VertexArrayObject::VertexArrayObject(unsigned int id)
	{
		setRendererID(id);
		glGenVertexArrays(1, &m_RendererID);
	}
	VertexArrayObject::~VertexArrayObject()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void VertexArrayObject::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void VertexArrayObject::Unbind() const
	{
		glBindVertexArray(0);
	}
	void VertexArrayObject::AddVertexBuffer(const VertexBuffer& vertexBuffer)
	{
		Bind();
		vertexBuffer.Bind();
		// Note: In a complete implementation, we would also set up vertex attribute pointers here.
	}
}
