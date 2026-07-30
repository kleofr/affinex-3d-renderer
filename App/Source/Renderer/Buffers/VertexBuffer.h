#pragma once

#include <stdint.h>
#include <glad/glad.h>

namespace AffineX
{
	class VertexBuffer
	{
	public:
		// Constructor to generate a new VBO
		VertexBuffer()=default;

		// Destructor to clean up the VBO
		~VertexBuffer();

		// Move constructor
		VertexBuffer(VertexBuffer&& other) noexcept
			: m_VBO(other.m_VBO) {
			other.m_VBO = 0;   // take ownership
		}

		// Move assignment operator
		VertexBuffer& operator=(VertexBuffer&& other) noexcept {
			if (this != &other) {
				if (m_VBO) glDeleteBuffers(1, &m_VBO); // delete current
				m_VBO = other.m_VBO;
				other.m_VBO = 0;
			}
			return *this;
		}

		// Delete copy constructor and copy assignment operator
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		void Create();
		void Create(unsigned int VBO);

		// Bind and Unbind the VBO
		void Bind() const;
		void Unbind() const;
		void addData(auto data, uint32_t size) const;

		// Getters and Setters for VBO
		void setVBO(unsigned int VBO) { m_VBO = VBO; };
		unsigned int getVBO() const { return m_VBO; };

	private:
		unsigned int m_VBO = 0;
	};
}