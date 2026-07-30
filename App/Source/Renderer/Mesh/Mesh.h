#pragma once

#include <vector>
#include <glad/glad.h>

#include "../Buffers/IndexBuffer.h"
#include "../Buffers/VertexBuffer.h"
#include "../Buffers/VertexArrayObject.h"

#include "../../Core/Logging/Log.h"


namespace AffineX {
    struct VertexAttribute {
        std::vector<float> data;      
        int                components;
        unsigned int       location;  
        GLenum             type;      
        GLboolean          normalized;
    };

    class Mesh {
    public:
		Mesh() = default; 

        // Constructor
        Mesh(const std::vector<VertexAttribute>& attributes,
            const std::vector<unsigned int>& indices,
            GLenum                              primitiveType,
            GLenum                              usageHint = GL_STATIC_DRAW);

        ~Mesh() = default;

		//=========================================================
		// Move Semantics
		//=========================================================

        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(Mesh&& other) noexcept;

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        void bind() const;
        void unbind() const;

        void draw() const;

        unsigned int getVAO() const;
        unsigned int getIndexCount() const;
        GLenum       getPrimitiveType() const;

    private:
        VertexArrayObject m_vao;
        std::vector<VertexBuffer> m_vbos;
        IndexBuffer m_ibo;
        unsigned int m_indexCount;
        GLenum m_primitiveType;
        GLenum m_usageHint;
		mutable bool firstDraw = true; 
    };
}