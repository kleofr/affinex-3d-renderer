#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include "ShaderFileReader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


namespace AffineX
{
	class Shader
	{
	public:

		Shader() = default;
		// Constructor that builds the shader from a file
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();
		// Activate the shader
		void use() const;
		// Utility functions to set uniform variables in the shader
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
	private:
		// The program ID
		unsigned int shaderID;
		// Utility function for checking shader compilation/linking errors.
		void checkCompileErrors(unsigned int shader, const std::string& type);
	};
}