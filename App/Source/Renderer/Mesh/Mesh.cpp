#include "Mesh.h"   // Adjust include path if needed

#include <glad/glad.h>

namespace AffineX {

    // ----------------------------------------------------------------------------
    // Constructor
    // ----------------------------------------------------------------------------
    Mesh::Mesh(const std::vector<VertexAttribute>& attributes,
        const std::vector<unsigned int>& indices,
        GLenum                              primitiveType,
        GLenum                              usageHint)
        : m_vbos()                             // empty, will be filled
        , m_indexCount(static_cast<unsigned int>(indices.size()))
        , m_primitiveType(primitiveType)
        , m_usageHint(usageHint)
    {
        m_vao.Create();
		m_ibo.Create();
        // Bind the VAO so all attribute configuration goes to it
        m_vao.Bind();

        // Reserve space for VBOs (one per attribute)
        m_vbos.reserve(attributes.size());

        // For each attribute, create a VBO, upload data, and set up the attribute pointer
        for (const auto& attr : attributes) {
            // Create a VBO (constructor generates buffer)
            VertexBuffer vbo;
			vbo.Create();
            // Upload the data
            vbo.Bind();
            glBufferData(GL_ARRAY_BUFFER,
                attr.data.size() * sizeof(float),
                attr.data.data(),
                m_usageHint);

            // Configure the attribute at the given location
            glVertexAttribPointer(attr.location,
                attr.components,
                attr.type,
                attr.normalized,
                0,               // tightly packed per attribute
                nullptr);
            glEnableVertexAttribArray(attr.location);

            vbo.Unbind();   // optional, VAO will remember the binding

            // Move the VBO into our vector
            m_vbos.emplace_back(std::move(vbo));
        }

        for (size_t i = 0; i < m_vbos.size(); ++i) {
            LOG_INFO("VBO[{}] ID: {}", i, m_vbos[i].getVBO());
        }

        // Upload the index data
        m_ibo.Bind();
        GLint currentIBO;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentIBO);
        LOG_INFO("Current IBO: {} (expected: {})", currentIBO, m_ibo.getIBO());

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(unsigned int),
            indices.data(),
            m_usageHint);
        //m_ibo.Unbind();
        //m_vao.Unbind();
    }

    // ----------------------------------------------------------------------------
    // Move semantics
    // ----------------------------------------------------------------------------
    Mesh::Mesh(Mesh&& other) noexcept
        : m_vao(std::move(other.m_vao))
        , m_vbos(std::move(other.m_vbos))
        , m_ibo(std::move(other.m_ibo))
        , m_indexCount(other.m_indexCount)
        , m_primitiveType(other.m_primitiveType)
        , m_usageHint(other.m_usageHint)
    {
        // Leave other in a valid but unspecified state – its resources are moved
    }

    Mesh& Mesh::operator=(Mesh&& other) noexcept {
        if (this != &other) {
            m_vao = std::move(other.m_vao);
            m_vbos = std::move(other.m_vbos);
            m_ibo = std::move(other.m_ibo);
            m_indexCount = other.m_indexCount;
            m_primitiveType = other.m_primitiveType;
            m_usageHint = other.m_usageHint;
        }
        return *this;
    }

    // ----------------------------------------------------------------------------
    // Binding
    // ----------------------------------------------------------------------------
    void Mesh::bind() const {
        m_vao.Bind();
    }

    void Mesh::unbind() const {
        m_vao.Unbind();
    }

    // ----------------------------------------------------------------------------
    // Drawing
    // ----------------------------------------------------------------------------
    void Mesh::draw() const {

        // Bind the VAO (this sets all VBOs and the IBO)
        m_vao.Bind();
        GLenum err;

        if (firstDraw) {
            GLint currentIBO;
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentIBO);
            LOG_TRACE("Current IBO: {} (expected: {})", currentIBO, m_ibo.getIBO());

            err = glGetError();
            if (err != GL_NO_ERROR) LOG_ERROR("After VAO bind: {}", err);

            GLint currentVAO;
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);

            LOG_TRACE("Current VAO: {} Expected: {}",
                currentVAO,
                m_vao.getVAO());

            // Check if vertex attribute 0 is enabled
            GLint attribEnabled;
            glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &attribEnabled);
            LOG_TRACE("Attribute 0 enabled: {}", attribEnabled);

            // Check the VBO binding for attribute 0
            GLint vboBinding;
            glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vboBinding);
            LOG_TRACE("VBO bound to attribute 0: {}", vboBinding);

            // Check the number of components
            GLint comps;
            glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_SIZE, &comps);
            LOG_TRACE("Attribute 0 components: {}", comps);

            // Check the index buffer is actually bound at draw time
            GLint currentEBO;
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentEBO);
            LOG_TRACE("Current EBO (GL_ELEMENT_ARRAY_BUFFER): {}", currentEBO);

			firstDraw = false;
        }

        // Draw indexed triangles (or whatever primitive type was set)
        glDrawElements(m_primitiveType,
            m_indexCount,
            GL_UNSIGNED_INT,
            nullptr);

        err = glGetError();
        if (err != GL_NO_ERROR) LOG_ERROR("After draw: {}", err);


        // Optionally unbind the VAO after drawing (not required, but good for cleanliness)
        // m_vao.Unbind();
    }

    // ----------------------------------------------------------------------------
    // Getters
    // ----------------------------------------------------------------------------
    unsigned int Mesh::getVAO() const {
        return m_vao.getVAO();   
    }

    unsigned int Mesh::getIndexCount() const {
        return m_indexCount;
    }

    GLenum Mesh::getPrimitiveType() const {
        return m_primitiveType;
    }

} // namespace AffineX