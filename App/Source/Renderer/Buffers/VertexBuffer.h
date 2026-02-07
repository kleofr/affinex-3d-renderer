#pragma once

#include <stdint.h>
#include <glad/glad.h>

namespace AffineX
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, uint32_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		unsigned int m_RendererID;
	};
}