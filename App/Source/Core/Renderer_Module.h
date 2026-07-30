#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <string>

// Debugging/logging
#include <Core/Logging/Log.h>

// Shader and buffer classes
#include <Renderer/Shaders/Shader.h>
#include <Renderer/Mesh/Mesh.h>
#include <Renderer/Camera/Camera.h>

struct GLFWwindow;

namespace AffineX
{

	class Renderer_Module
	{
	public:
		Renderer_Module() = default;
		virtual ~Renderer_Module();


		bool InitRenderer(GLFWwindow& window);
		void ShutdownRenderer();

		void BeginFrame();
		void EndFrame();

		void MakeScene();
		void DrawScene();

		void SetClearColor(float r, float g, float b, float a = 1.0f);
		void SetViewport(int width, int height);

		GLuint CreateShaderProgram();

		bool IsInitialized() const noexcept { return m_initialized; }

	private:
		GLFWwindow* m_window = nullptr;
		bool m_initialized = false;
		float m_clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		mutable bool firstDraw = true;

		// Debugging/test resources (for initial testing of rendering pipeline)
		Mesh m_testMesh;
		Shader m_testShader;
		GLuint m_testProgram = 0;
	};
}

