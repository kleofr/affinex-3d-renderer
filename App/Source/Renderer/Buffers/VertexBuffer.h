#pragma once

#include <stdint.h>
#include <glad/glad.h>

namespace AffineX
{
	class VertexBuffer
	{
	public:
		VertexBuffer(GLuint &VBO);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
		void addData(auto data, uint32_t size) const;

		void setRendererID(GLuint id) { m_RendererID = id; }

	private:
		unsigned int m_RendererID;
	};
}