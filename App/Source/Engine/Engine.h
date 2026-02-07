//#include "Core/C_System.h"
#include <vector>
#include <memory>
#include "Core/Renderer_Module.h"
#include "Core/GUI_Module.h"
#include "Core/Window_Module.h"

namespace AffineX
{
	class Engine
	{
	public:
		// Constructor and Destructor
		Engine() = default;
		virtual ~Engine();

		// Public methods
		void Init();
		void Run();
		void Shutdown();

		//void addSystem(C_System& system); // Placeholder for future system additions

	private:
		Renderer_Module m_renderer;
		GUI_Module m_gui;
		Window_Module m_window;
		bool isRunning = false;
		Window_Module::Window_Data m_windowData{ "AffineX Engine", 800, 600, true };
		//std::vector<std::unique_ptr<C_System>> m_systems; // Placeholder for future systems
	}; 

}