#include "Engine.h"

namespace AffineX 
{
	

	Engine::~Engine() 
	{
		Shutdown();
	}

	void Engine::Init() 
	{
		m_window.InitWindow(m_windowData);
		m_gui.InitGUI(*m_window.GetGLFWwindow());
		m_logger.init();
		if (!m_renderer.InitRenderer(*m_window.GetGLFWwindow())) 
		{
			printf("Failed to initialize Renderer_Module\n");
			return;
		}
		isRunning = true;
	}

	void Engine::Run() 
	{
		m_renderer.createTriangle();
		while (isRunning && !glfwWindowShouldClose(m_window.GetGLFWwindow())) 
		{
			glfwPollEvents();
			m_renderer.BeginFrame();
			m_renderer.renderTraingle();
			m_gui.RenderGUI();
			m_renderer.EndFrame();
			glfwSwapBuffers(m_window.GetGLFWwindow());
		}
	}

	void Engine::Shutdown() 
	{
		//m_logger.shutdown();
		//m_renderer.ShutdownRenderer();
		//m_gui.ShutdownGUI();
		//m_window.ShutdownWindow();
	}
}