#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VertexBuffer.h"

namespace AffineX
{
	class VertexArrayObject
	{
	public:
		VertexArrayObject() = default;
		~VertexArrayObject();

		//=========================================================
		// Move Semantics
		//=========================================================

		VertexArrayObject(VertexArrayObject&& other) noexcept
			: m_VAO(other.m_VAO) {
			other.m_VAO = 0;
		}

		VertexArrayObject& operator=(VertexArrayObject&& other) noexcept {
			if (this != &other) {
				if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
				m_VAO = other.m_VAO;
				other.m_VAO = 0;
			}
			return *this;
		}

		VertexArrayObject(const VertexArrayObject&) = delete;
		VertexArrayObject& operator=(const VertexArrayObject&) = delete;

		void Create();
		void Create(unsigned int VAO);

		void Bind() const;
		void Unbind() const;

		void setVAO(unsigned int VAO) { m_VAO = VAO; };
		unsigned int getVAO() const { return m_VAO; };

	private:
		unsigned int m_VAO = 0;
	};
}