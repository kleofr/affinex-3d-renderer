#include "VertexArrayObject.h"

namespace AffineX
{

	void VertexArrayObject::Create()
	{
		glGenVertexArrays(1, &m_VAO);
	}

	void VertexArrayObject::Create(unsigned int VAO)
	{
		glGenVertexArrays(1, &m_VAO);
	}

	VertexArrayObject::~VertexArrayObject()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}
	void VertexArrayObject::Bind() const
	{
		glBindVertexArray(m_VAO);
	}
	void VertexArrayObject::Unbind() const
	{
		glBindVertexArray(0);
	}
}
