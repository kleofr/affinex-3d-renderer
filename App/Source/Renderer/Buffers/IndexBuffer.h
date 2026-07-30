#pragma once

#include <stdint.h>
#include <glad/glad.h>

namespace AffineX
{
	class IndexBuffer
	{
	public:
		IndexBuffer()=default;
		~IndexBuffer();

		// Move constructor
		IndexBuffer(IndexBuffer&& other) noexcept
			: m_IBO(other.m_IBO), m_Count(other.m_Count) {
			other.m_IBO = 0;   // take ownership
			other.m_Count = 0;
		}

		// Move assignment operator
		IndexBuffer& operator=(IndexBuffer&& other) noexcept {
			if (this != &other) {
				if (m_IBO) glDeleteBuffers(1, &m_IBO); // delete current
				m_IBO = other.m_IBO;
				m_Count = other.m_Count;
				other.m_IBO = 0;
				other.m_Count = 0;
			}
			return *this;
		}

		// Delete copy constructor and copy assignment operator
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		void Create();
		void Create(unsigned int IBO);
		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const { return m_Count; }
		void setIBO(unsigned int IBO) { m_IBO = IBO; }
		unsigned int getIBO() const { return m_IBO; }

	private:

		unsigned int m_IBO = 0;
		uint32_t m_Count = 0;

	};
}