#include <algorithm>

//	Modules
#include "GUI_Module.h"
#include "Window_Module.h"

// Include ImGui headers
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace AffineX
{

	void GUI_Module::InitGUI(GLFWwindow& window)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		LOG_INFO("GUI::Context successfully created.");

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
		LOG_INFO("GUI::Docking enabled.");

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(&window, true);          // Pass pointer to GLFWwindow
		ImGui_ImplOpenGL3_Init();
		LOG_INFO("GUI::Platform/Renderer backends initialized.");
	}

	void GUI_Module::RenderGUI()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
        
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), dockspace_flags);
		
		ApplyStyle(); // Apply custom styling for the editor
        RenderViewport();
        RenderDetails();
		RenderConsole(m_logStorage, m_filterBuffer, sizeof(m_filterBuffer));
		
        
        //renderDebugConsole(); // Render the debug console panel

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GUI_Module::ShutdownGUI()
	{
		// Cleanup ImGui and backends
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

    // ------------------------------------------------------------------------
    // Helper: Convert LogLevel to ImGui color
    // ------------------------------------------------------------------------
    static ImVec4 getSeverityColor(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Trace:    return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);   // Grey
        case LogLevel::Debug:    return ImVec4(0.3f, 0.7f, 0.9f, 1.0f);   // Light Blue
        case LogLevel::Info:     return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // White
        case LogLevel::Warn:     return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);   // Yellow
        case LogLevel::Error:    return ImVec4(1.0f, 0.4f, 0.2f, 1.0f);   // Orange-Red
        case LogLevel::Critical: return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);   // Bright Red
        default:                 return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }

    // ------------------------------------------------------------------------
    // Helper: Check if a severity level should be shown based on filters
    // ------------------------------------------------------------------------
    static bool isLevelVisible(LogLevel level,
        bool showTrace, bool showDebug, bool showInfo,
        bool showWarn, bool showError, bool showCritical)
    {
        switch (level)
        {
        case LogLevel::Trace:    return showTrace;
        case LogLevel::Debug:    return showDebug;
        case LogLevel::Info:     return showInfo;
        case LogLevel::Warn:     return showWarn;
        case LogLevel::Error:    return showError;
        case LogLevel::Critical: return showCritical;
        default:                 return true;
        }
    }

}