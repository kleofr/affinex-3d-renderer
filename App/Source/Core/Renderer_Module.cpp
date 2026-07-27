#include "Renderer_Module.h"
#include "Renderer/Buffers/VertexBuffer.h"

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
			LOG_ERROR("Renderer_Module: Failed to initialize GLAD");
			m_window = nullptr;
			return false;
		}

		const char* glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		const char* glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

		// Print GL info for diagnostics
		LOG_INFO("OpenGL: {} | GLSL: {}", glVersion, glslVersion);

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

	void Renderer_Module::createTriangle() const
	{
		if (!m_initialized) return;
		// Simple hardcoded triangle rendering for testing
		float vertices[] = {
			0.0f,  0.5f, 0.0f,
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
		};
		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);


		glUseProgram(0); // Use fixed-function pipeline for simplicity
	}

	void Renderer_Module::renderTraingle() const
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	int Renderer_Module::createShaderProgram(const std::string* vertexSrc, const std::string* fragSrc)
	{
		
		return 0;
	}

	void Renderer_Module::SetViewport(int width, int height)
	{
		if (!m_initialized) return;
		glViewport(0, 0, width, height);
	}
}
