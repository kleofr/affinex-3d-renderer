#pragma once

#include <glad/glad.h>
#include "VertexBuffer.h"

namespace AffineX
{
	class VertexArrayObject
	{
	public:
		VertexArrayObject();
		~VertexArrayObject();
		void Bind() const;
		void Unbind() const;
		void AddVertexBuffer(const VertexBuffer& vertexBuffer);

	private:
		unsigned int m_RendererID;
	};
}