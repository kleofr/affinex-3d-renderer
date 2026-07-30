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
		//Constructor to generate a new VAO
		VertexArrayObject() = default;

		// Destructor to clean up the VAO
		~VertexArrayObject();

		// Move constructor
		VertexArrayObject(VertexArrayObject&& other) noexcept
			: m_VAO(other.m_VAO) {
			other.m_VAO = 0;   // take ownership
		}

		VertexArrayObject& operator=(VertexArrayObject&& other) noexcept {
			if (this != &other) {
				if (m_VAO) glDeleteVertexArrays(1, &m_VAO); // delete current
				m_VAO = other.m_VAO;
				other.m_VAO = 0;
			}
			return *this;
		}

		// Delete copy
		VertexArrayObject(const VertexArrayObject&) = delete;
		VertexArrayObject& operator=(const VertexArrayObject&) = delete;

		void Create();
		void Create(unsigned int VAO);

		// Bind and Unbind the VAO
		void Bind() const;
		void Unbind() const;

		//Getters and Setters for VAO
		void setVAO(unsigned int VAO) { m_VAO = VAO; };
		unsigned int getVAO() const { return m_VAO; };

	private:
		unsigned int m_VAO = 0;
	};
}