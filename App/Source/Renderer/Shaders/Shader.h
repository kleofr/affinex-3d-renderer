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
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void use() const;

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
	private:

		unsigned int shaderID;
		void checkCompileErrors(unsigned int shader, const std::string& type);
	};
}