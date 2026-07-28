#pragma once

#include <glad/glad.h>
#include "VertexBuffer.h"

namespace AffineX
{
	class VertexArrayObject
	{
	public:
		VertexArrayObject(unsigned int id);
		~VertexArrayObject();
		void Bind() const;
		void Unbind() const;
		void AddVertexBuffer(const VertexBuffer& vertexBuffer);
		void setRendererID(GLuint id) { m_RendererID = id; };

	private:
		unsigned int m_RendererID = 0;
		unsigned int m_VBO; // Store the VBO ID for potential future use
	};
}