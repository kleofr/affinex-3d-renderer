#include <GLFW/glfw3.h>
#include "Logging/LogStorage.h"

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
		// --- New: Render the debug console panel ---
		void renderDebugConsole();

		// --- New: Internal state for the console ---
		LogStorage* m_logStorage = nullptr;
		char m_filterBuffer[256] = { 0 };           // Filter text input
		bool m_autoScroll = true;                   // Auto-scroll to bottom
		bool m_showTrace = true;                    // Filter by severity
		bool m_showDebug = true;
		bool m_showInfo = true;
		bool m_showWarn = true;
		bool m_showError = true;
		bool m_showCritical = true;
	};
}