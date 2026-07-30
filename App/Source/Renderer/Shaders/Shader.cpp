// Shader.cpp
#include "Shader.h"

#include <glad/glad.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

namespace AffineX {

    // ------------------------------------------------------------
    // Constructor
    // ------------------------------------------------------------
    Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
        : shaderID(0)
    {
        // 1. Retrieve the source code from file paths
        std::string vertexCode = read_file(vertexPath);
        std::string fragmentCode = read_file(fragmentPath);

        if (vertexCode.empty() || fragmentCode.empty()) {
            LOG_ERROR("Failed to read shader source files.");
            return;
        }

        // 2. Compile shaders
        unsigned int vertex, fragment;
        const char* vCode = vertexCode.c_str();
        const char* fCode = fragmentCode.c_str();
        LOG_INFO("Shader: Fetched source code from files.");

        // Vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vCode, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
		LOG_INFO("Shader: Vertex shader compiled successfully.");

        // Fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
		LOG_INFO("Shader: Fragment shader compiled successfully.");

        // 3. Link shaders into a program
        shaderID = glCreateProgram();
        glAttachShader(shaderID, vertex);
        glAttachShader(shaderID, fragment);
        glLinkProgram(shaderID);
        checkCompileErrors(shaderID, "PROGRAM");
		LOG_INFO("Shader: Shader program linked successfully.");

        // 4. Delete the shader objects (they are no longer needed)
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        LOG_INFO("Shader: Shader program created successfully (ID: {})", shaderID);
    }

    // ------------------------------------------------------------
    // Destructor
    // ------------------------------------------------------------
    Shader::~Shader() {
        if (shaderID != 0) {
            glDeleteProgram(shaderID);
        }
    }

    // ------------------------------------------------------------
    // Activate the shader
    // ------------------------------------------------------------
    void Shader::use() const {
        if (shaderID != 0) {
            glUseProgram(shaderID);
        }
        else {
            LOG_ERROR("Shader: Attempted to use an invalid shader program.");
        }
    }

    // ------------------------------------------------------------
    // Utility uniform setters
    // ------------------------------------------------------------
    void Shader::setBool(const std::string& name, bool value) const {
        if (shaderID == 0) {
            LOG_ERROR("Shader: Cannot set uniform '{}' on invalid shader.", name);
            return;
        }
        int location = glGetUniformLocation(shaderID, name.c_str());
        glUniform1i(location, static_cast<int>(value));
    }

    void Shader::setInt(const std::string& name, int value) const {
        if (shaderID == 0) {
            LOG_ERROR("Shader: Cannot set uniform '{}' on invalid shader.", name);
            return;
        }
        int location = glGetUniformLocation(shaderID, name.c_str());
        glUniform1i(location, value);
    }

    void Shader::setFloat(const std::string& name, float value) const {
        if (shaderID == 0) {
            LOG_ERROR("Shader: Cannot set uniform '{}' on invalid shader.", name);
            return;
        }
        int location = glGetUniformLocation(shaderID, name.c_str());
        glUniform1f(location, value);
    }

    // ------------------------------------------------------------
    // Private helper: check compilation / linking errors
    // ------------------------------------------------------------
    void Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
        int success;
        char infoLog[1024];

        if (type == "PROGRAM") {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                LOG_ERROR("Shader: Shader program linking error:\n{}", infoLog);
            }
        }
        else {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                LOG_ERROR("Shader: Shader compilation error ({}) :\n{}", type, infoLog);
            }
        }
    }

} // namespace AffineX