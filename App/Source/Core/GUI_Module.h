#include <GLFW/glfw3.h>

namespace AffineX
{
	class GUI_Module
	{

	public:

		GUI_Module() = default;
		virtual ~GUI_Module() = default;

		void InitGUI(GLFWwindow& window);
		void RenderGUI();
		void ShutdownGUI();


	};
}