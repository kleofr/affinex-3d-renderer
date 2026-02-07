#pragma once

#include <stdint.h>
#include <glad/glad.h>

namespace AffineX
{
	class IndexBuffer
	{
	public:

		IndexBuffer(const uint32_t* data, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const { return m_Count; }

	private:

		unsigned int m_RendererID;
		uint32_t m_Count;

	};
}