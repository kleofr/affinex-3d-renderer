#include <GLFW/glfw3.h>
#include "Logging/Log.h"
#include "Logging/LogStorage.h"
#include "Editor/EditorUI.h"

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

		void SetLogStorage(LogStorage* logStorage) { m_logStorage = logStorage; }

	private:
		// --- New: Internal state for the console ---
		LogStorage* m_logStorage = nullptr;
		char m_filterBuffer[256] = { 0 };           // Filter text input
	};
}