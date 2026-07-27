#include "Engine.h"
#include "../Core/Logging/Log.h"

namespace AffineX 
{
	

	Engine::~Engine() 
	{
		Shutdown();
	}

	void Engine::Init() 
	{
		m_logger.init();

		m_window.InitWindow(m_windowData);
		LOG_INFO("Window initialized: {} ({}x{})", m_windowData.Title, m_windowData.Width, m_windowData.Height);
		m_gui.InitGUI(*m_window.GetGLFWwindow());
		LOG_INFO("GUI initialized");
		m_gui.SetLogStorage(m_logger.getStorage());
		LOG_INFO("Logger initialized");
		if (!m_renderer.InitRenderer(*m_window.GetGLFWwindow())) 
		{
			LOG_ERROR("Failed to initialize Renderer_Module");
			return;
		}
		isRunning = true;
		LOG_INFO("Engine initialized");
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