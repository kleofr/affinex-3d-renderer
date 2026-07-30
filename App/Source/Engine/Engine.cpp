#include "Engine.h"

namespace AffineX 
{
	

	Engine::~Engine() 
	{
		Shutdown();
	}

	void Engine::Init() 
	{
		m_logger.init();
		LOG_INFO("========== AffineX Engine ==========");
		m_window.InitWindow(m_windowData);
		LOG_INFO("Engine: Window Module initialized: {} ({}x{})", m_windowData.Title, m_windowData.Width, m_windowData.Height);
		m_gui.InitGUI(*m_window.GetGLFWwindow());
		LOG_INFO("Engine: GUI Module initialized");
		m_gui.SetLogStorage(m_logger.getStorage());
		LOG_INFO("Engine: Logger Module initialized");
		LOG_INFO("Engine: Log Storage set for GUI Module");
		if (!m_renderer.InitRenderer(*m_window.GetGLFWwindow())) 
		{
			LOG_ERROR("Engine: Failed to initialize Renderer_Module");
			return;
		}
		isRunning = true;
		LOG_INFO("Engine: Initialized and Running");

		m_renderer.MakeScene(); // Create a simple test scene for rendering

	}

	void Engine::Run() 
	{
		while (isRunning && !glfwWindowShouldClose(m_window.GetGLFWwindow())) 
		{
			glfwPollEvents();
			m_renderer.BeginFrame();

			m_renderer.DrawScene(); // Draw the test scene

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