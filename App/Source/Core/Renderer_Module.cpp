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


	void Renderer_Module::SetClearColor(float r, float g, float b, float a)
	{
		m_clearColor[0] = r;
		m_clearColor[1] = g;
		m_clearColor[2] = b;
		m_clearColor[3] = a;
	}

	void Renderer_Module::renderTraingle() const
	{
		if (!m_initialized) return;
		// Simple hardcoded triangle rendering for testing
		float vertices[] = {
			0.0f,  0.5f, 0.0f,
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
		};
		GLuint VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glUseProgram(0); // Use fixed-function pipeline for simplicity
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}

	void Renderer_Module::SetViewport(int width, int height)
	{
		if (!m_initialized) return;
		glViewport(0, 0, width, height);
	}
}
