#pragma once

#include <string>
#include <cstdio>

struct GLFWwindow; // Forward declaration

namespace AffineX
{
	class Window_Module
	{
	private:

		// Member variables
		GLFWwindow* m_Window = nullptr;
		std::string m_Title;
		unsigned int m_Width = 0, m_Height = 0;
		bool m_VSync = true;

	public:

		// Data structure to hold window properties
		struct Window_Data
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
		};

		// Constructor and Destructor
		Window_Module(const Window_Data& data)
			: m_Title(data.Title), m_Width(data.Width), m_Height(data.Height), m_VSync(data.VSync)
		{
		}
		virtual ~Window_Module();

		// Public methods
		virtual void InitWindow();
		virtual void RunWindow();
		virtual void ShutdownWindow();

		// Getters
		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
		inline bool IsVSync() const { return m_VSync; }

		inline GLFWwindow* GetGLFWwindow() const { return m_Window; }


	};
}


