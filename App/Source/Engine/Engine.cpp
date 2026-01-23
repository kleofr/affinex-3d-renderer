#include "Engine.h"

namespace AffineX 
{
	Engine::~Engine() 
	{
		Shutdown();
	}

	void Engine::Init(const Window_Module::Window_Data& windowData) 
	{

		// Initialize Window module
		m_window = std::make_unique<Window_Module>(windowData);
		m_window->InitWindow();
		GLFWwindow* window = m_window->GetGLFWwindow();

		if (!window)
		{
			printf("Failed to create GLFW window\n");
			return;
		}

		// Initialize GUI module
		m_gui.InitGUI(*window);

		if (!m_renderer.InitRenderer(*window))
		{
			printf("Failed to initialize Renderer Module\n");
			return;
		}
		isRunning = true;
	}

	void Engine::Run() 
	{
		while (isRunning && !glfwWindowShouldClose(m_window->GetGLFWwindow())) 
		{
			glfwPollEvents();
			m_renderer.BeginFrame();
			m_gui.RenderGUI();
			m_renderer.EndFrame();
			glfwSwapBuffers(m_window->GetGLFWwindow());
		}
	}

	void Engine::Shutdown() 
	{
		// Shutdown GUI module
		m_gui.ShutdownGUI();
		// Shutdown Window module
		if (m_window) 
		{
			m_window->ShutdownWindow();
			m_window.reset();
		}
		isRunning = false;
	}
}