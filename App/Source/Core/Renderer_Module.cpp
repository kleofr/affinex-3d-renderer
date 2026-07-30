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

		// ==================================================
		// Load GL function pointers with GLAD using GLFW's loader.
		// ==================================================

		m_window = &window;

		// Load GL function pointers with GLAD using GLFW's loader.
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Renderer_Module: Failed to initialize GLAD");
			m_window = nullptr;
			return false;
		}

		if (!gladLoadGL()) {
			LOG_ERROR("Failed to load OpenGL functions!");
			return false;
		}

		// ==================================================
		// Print GL info for diagnostics
		// ==================================================

		const char* glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		const char* glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));


		LOG_INFO("OpenGL: {} | GLSL: {}", glVersion, glslVersion);

		// Configure default GL state
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		LOG_INFO("Renderer_Module: Depth testing enabled (GL_LESS)");

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		LOG_INFO("Renderer_Module: Back-face culling enabled");

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		LOG_INFO("Renderer_Module: Blending enabled");

		// ==================================================
		// Set default viewport and clear color
		// ==================================================

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
		// ===================================================
		// Cleanup and reset state
		// ===================================================

		if (!m_initialized) return;
		
		glFinish();

		m_window = nullptr;
		m_initialized = false;
	}

	void Renderer_Module::BeginFrame()
	{
		//=================================================
		// Clear color and depth buffers for a new frame
		//=================================================

		if (!gladLoadGL()) return;

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
		//=================================================
		// Flush GL commands for the current frame
		//=================================================

		if (!gladLoadGL()) return;
		glFlush();
	}



	void Renderer_Module::SetClearColor(float r, float g, float b, float a)
	{
		//=================================================
		// Set the clear color for the renderer
		//=================================================

		m_clearColor[0] = r;
		m_clearColor[1] = g;
		m_clearColor[2] = b;
		m_clearColor[3] = a;
	}
	

	void Renderer_Module::SetViewport(int width, int height)
	{
		//=================================================
		// Set the viewport dimensions for rendering
		//=================================================

		if (!m_initialized) return;
		glViewport(0, 0, width, height);
	}

	void Renderer_Module::MakeScene()
	{
		//=================================================
		// Create a simple scene with a test mesh and shader program
		//=================================================
		
		if (!gladLoadGL()) return;
		m_testMesh = Mesh(
			{
				// Cube vertices (8 vertices * 4 floats each = 32 floats)
				{ {
					-0.5f, -0.5f, -0.5f, 1.0f,   // 0
					 0.5f, -0.5f, -0.5f, 1.0f,   // 1
					 0.5f,  0.5f, -0.5f, 1.0f,   // 2
					-0.5f,  0.5f, -0.5f, 1.0f,   // 3
					-0.5f, -0.5f,  0.5f, 1.0f,   // 4
					 0.5f, -0.5f,  0.5f, 1.0f,   // 5
					 0.5f,  0.5f,  0.5f, 1.0f,   // 6
					-0.5f,  0.5f,  0.5f, 1.0f    // 7
				}, // data
				4,    // components (x, y, z, w)
				0,    // location (matches shader layout)
				GL_FLOAT,
				GL_FALSE }
			},
	{   // Indices for 12 triangles (36 indices)
		// Back face (z = -0.5)
		0, 1, 2,   0, 2, 3,
		// Front face (z = +0.5)
		4, 6, 5,   4, 7, 6,
		// Left face (x = -0.5)
		0, 3, 7,   0, 7, 4,
		// Right face (x = +0.5)
		1, 5, 6,   1, 6, 2,
		// Bottom face (y = -0.5)
		0, 4, 5,   0, 5, 1,
		// Top face (y = +0.5)
		3, 2, 6,   3, 6, 7
	},
			GL_TRIANGLES
		);

		m_testProgram = CreateShaderProgram();
		//m_testShader = Shader("path/to/vertex_shader.glsl", "path/to/fragment_shader.glsl");
	}

	void Renderer_Module::DrawScene()
	{
		//=================================================
		// Draw the test mesh using the test shader program
		//=================================================

		if (!gladLoadGL()) return;
		if (!m_initialized) return;
		glUseProgram(m_testProgram);
		GLint currentProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		if (firstDraw) {
			LOG_TRACE("Current program: {} (expected: {})", currentProgram, m_testProgram);
			firstDraw = false;
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		m_testMesh.draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // revert
		//m_testShader.use();
	}


	GLuint Renderer_Module::CreateShaderProgram()
	{
		//=================================================
		// Create a simple shader program for testing
		//=================================================

		const char* vertexSource = R"(
        #version 330 core
        layout (location = 0) in vec4 aPos;
			void main() {
				gl_Position = aPos;
			}
		)";
		const char* fragmentSource = R"(
        #version 330 core
        out vec4 FragColor;
			void main() {
				FragColor = vec4(1.0, 0.5, 0.2, 1.0); // orange
			}
		)";

		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, nullptr);
		glCompileShader(vertex);
		// (add error checking if you like)

		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, nullptr);
		glCompileShader(fragment);

		GLuint program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
		// (error checking)

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return program;

		int success;
		char infoLog[512];
		glGetProgramiv(m_testProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_testProgram, 512, nullptr, infoLog);
			LOG_ERROR("Shader linking failed:\n{}", infoLog);
		}
		else {
			LOG_INFO("Shader program linked successfully (ID: {})", m_testProgram);
		}

	}
}
