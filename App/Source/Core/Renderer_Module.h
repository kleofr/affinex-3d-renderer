#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>

struct GLFWwindow; // forward-declare so header doesn't need glfw include

namespace AffineX
{
	//class R_ShaderSystem;
	//class R_MeshSystem;
	//class R_MeshRenderer;

	class Renderer_Module
	{
	public:
		Renderer_Module() = default;
		virtual ~Renderer_Module();

		// Initialize GL using an existing GLFW context (reference used per project preference).
		// Returns true on success (GLAD loaded & basic state set).
		bool InitRenderer(GLFWwindow& window);

		// Tear down/cleanup (idempotent).
		void ShutdownRenderer();

		// Begin a frame (clears color/depth buffers).
		void BeginFrame();

		// End a frame (flush GL commands). Does NOT swap buffers — let windowing layer handle that.
		void EndFrame();

		void renderTraingle() const;

		void SetClearColor(float r, float g, float b, float a = 1.0f);
		void SetViewport(int width, int height);

		bool IsInitialized() const noexcept { return m_initialized; }

	private:
		GLFWwindow* m_window = nullptr;
		bool m_initialized = false;
		float m_clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	};
}

