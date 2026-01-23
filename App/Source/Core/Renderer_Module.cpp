#include "Renderer_Module.h"

#include <cstdio>

namespace AffineX
{
	Renderer_Module::~Renderer_Module()
	{
		ShutdownRenderer();
	}

	bool Renderer_Module::InitRenderer(GLFWwindow& window)
	{
		if (m_initialized) return true;

		// Store pointer for queries (framebuffer size). We do not own the window.
		m_window = &window;

		// Load GL function pointers with GLAD using GLFW's loader.
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::fprintf(stderr, "Renderer_Module: Failed to initialize GLAD\n");
			m_window = nullptr;
			return false;
		}

		// Print GL info for diagnostics
		std::printf("OpenGL: %s | GLSL: %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

		// Configure default GL state
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Set viewport to the current framebuffer size
		int fbw = 0, fbh = 0;
		glfwGetFramebufferSize(m_window, &fbw, &fbh);
		glViewport(0, 0, fbw, fbh);

		// Default clear color
		glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);

		m_initialized = true;
		return true;
	}

	void Renderer_Module::ShutdownRenderer()
	{
		if (!m_initialized) return;

		// No owned GL resources here (caller manages VAOs/VBOs/shaders).
		// Issue a finish to ensure pending commands complete before shutdown if needed.
		glFinish();

		m_window = nullptr;
		m_initialized = false;
	}

	void Renderer_Module::BeginFrame()
	{
		if (!m_initialized) return;

		// Update viewport in case of resize
		if (m_window)
		{
			int fbw = 0, fbh = 0;
			glfwGetFramebufferSize(m_window, &fbw, &fbh);
			glViewport(0, 0, fbw, fbh);
		}

		glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer_Module::EndFrame()
	{
		if (!m_initialized) return;

		// Flush commands; swapping buffers should be handled by the windowing layer to keep responsibilities separate.
		glFlush();
	}

	void Renderer_Module::DrawIndexed(GLuint vao, GLsizei indexCount, GLenum mode)
	{
		if (!m_initialized) return;
		glBindVertexArray(vao);
		// Assumes element type is unsigned int (common). Caller must ensure index buffer and program are bound.
		glDrawElements(mode, indexCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	void Renderer_Module::DrawArrays(GLuint vao, GLsizei count, GLenum mode)
	{
		if (!m_initialized) return;
		glBindVertexArray(vao);
		glDrawArrays(mode, 0, count);
		glBindVertexArray(0);
	}

	void Renderer_Module::SetClearColor(float r, float g, float b, float a)
	{
		m_clearColor[0] = r;
		m_clearColor[1] = g;
		m_clearColor[2] = b;
		m_clearColor[3] = a;
	}

	void Renderer_Module::SetViewport(int width, int height)
	{
		if (!m_initialized) return;
		glViewport(0, 0, width, height);
	}
}
