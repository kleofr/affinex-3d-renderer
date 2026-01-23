#include "Core/Renderer_Module.h"
#include "Core/Window_Module.h"
#include "Core/GUI_Module.h"
#include <memory>

namespace AffineX
{
	class Engine
	{
	public:
		// Constructor and Destructor
		Engine() = default;
		virtual ~Engine();

		// Public methods
		void Init(const Window_Module::Window_Data& windowData);
		void Run();
		void Shutdown();

	private:
		bool isRunning = false;
		std::unique_ptr<Window_Module> m_window;
		GUI_Module m_gui;
		Renderer_Module m_renderer;
	};
}