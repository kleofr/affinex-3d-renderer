

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window_Module.h"

namespace AffineX 
{
	Window_Module::~Window_Module() 
	{
		ShutdownWindow();
	}

	void Window_Module::InitWindow() 
	{
		if (!glfwInit()) 
		{
			printf("Failed to initialize GLFW\n");
			return;
		}
		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
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