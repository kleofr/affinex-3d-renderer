#pragma once

#include <vector>
#include <glad/glad.h>

#include "../Buffers/IndexBuffer.h"
#include "../Buffers/VertexBuffer.h"
#include "../Buffers/VertexArrayObject.h"

#include "../../Core/Logging/Log.h"


namespace AffineX {
    // ----------------------------------------------------------------------------
//  VertexAttribute – describes one vertex attribute (position, normal, UV, …)
// ----------------------------------------------------------------------------
    struct VertexAttribute {
        std::vector<float> data;      // the actual vertex data (tightly packed)
        int                components;// 1, 2, 3, or 4
        unsigned int       location;  // shader layout location (0, 1, 2, …)
        GLenum             type;      // usually GL_FLOAT
        GLboolean          normalized;// GL_TRUE or GL_FALSE
    };

    // ----------------------------------------------------------------------------
    // Mesh – owns VAO, separate VBOs per attribute, and an IBO.
    //        Movable but not copyable.
    // ----------------------------------------------------------------------------
    class Mesh {
    public:
		Mesh() = default; // default constructor for empty mesh

        // Constructor
        Mesh(const std::vector<VertexAttribute>& attributes,
            const std::vector<unsigned int>& indices,
            GLenum                              primitiveType,
            GLenum                              usageHint = GL_STATIC_DRAW);

        // Destructor – default is fine because wrappers clean up themselves
        ~Mesh() = default;

        // Move semantics (copy is deleted to avoid double ownership of GL resources)
        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(Mesh&& other) noexcept;

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        // Bind the VAO (and optionally unbind)
        void bind() const;
        void unbind() const;

        // Draw the mesh (assumes VAO is bound, or binds it internally)
        void draw() const;

        // Getters (for debugging / advanced use)
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

        //helper var
		mutable bool firstDraw = true; // to log only on the first draw call
    };
}