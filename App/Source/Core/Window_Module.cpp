

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window_Module.h"

namespace AffineX 
{
	Window_Module::~Window_Module() 
	{
		printf("Shutting down Window_Module\n");
		ShutdownWindow();
	}

	void Window_Module::InitWindow(Window_Data& data)
	{
		if (!glfwInit()) 
		{
			printf("Failed to initialize GLFW\n");
			return;
		}
		m_Window = glfwCreateWindow(data.Width, data.Height, data.Title.c_str(), nullptr, nullptr);
		if (!m_Window) 
		{
			printf("Failed to create GLFW window\n");
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_Window);
		if (m_VSync) 
		{
			glfwSwapInterval(1); // Enable VSync
		} 
		else 
		{
			glfwSwapInterval(0); // Disable VSync
		}
		printf("Window '%s' created with size %ux%u\n", data.Title.c_str(), data.Width, data.Height);
	}
	void Window_Module::RunWindow() 
	{
		if (m_Window) 
		{
			glfwPollEvents();
			glfwSwapBuffers(m_Window);
		}
	}

	void Window_Module::ShutdownWindow() 
	{
		if (m_Window) 
		{
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}
		glfwTerminate();
	}
}