#pragma once

#include <stdint.h>
#include <glad/glad.h>

namespace AffineX
{
	class VertexBuffer
	{
	public:
		VertexBuffer()=default;
		~VertexBuffer();
		
		//=============================================================
		// Move Semantics
		//=============================================================
		VertexBuffer(VertexBuffer&& other) noexcept
			: m_VBO(other.m_VBO) {
			other.m_VBO = 0;
		}

		VertexBuffer& operator=(VertexBuffer&& other) noexcept {
			if (this != &other) {
				if (m_VBO) glDeleteBuffers(1, &m_VBO);
				m_VBO = other.m_VBO;
				other.m_VBO = 0;
			}
			return *this;
		}

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		void Create();
		void Create(unsigned int VBO);

		void Bind() const;
		void Unbind() const;

		void setVBO(unsigned int VBO) { m_VBO = VBO; };
		unsigned int getVBO() const { return m_VBO; };

	private:
		unsigned int m_VBO = 0;
	};
}