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
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(&window, true);          // Pass pointer to GLFWwindow
		ImGui_ImplOpenGL3_Init();
	}

	void GUI_Module::RenderGUI()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
        
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), dockspace_flags);
		renderDebugConsole(); // Render the debug console panel

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

    // ------------------------------------------------------------------------
    // Main debug console rendering
    // ------------------------------------------------------------------------
    void GUI_Module::renderDebugConsole()
    {
        if (!m_logStorage)
            return;  // No storage available yet

        // Begin the debug console window
        ImGui::Begin("Debug Console");

        // ------------------------------------------------------------
        // 1. Control bar (filter + severity toggles + utilities)
        // ------------------------------------------------------------
        ImGui::BeginGroup();

        // --- Filter text input ---
        ImGui::InputText("Filter", m_filterBuffer, sizeof(m_filterBuffer));
        ImGui::SameLine();

        // --- Auto-scroll toggle ---
        ImGui::Checkbox("Auto-scroll", &m_autoScroll);
        ImGui::SameLine();

        // --- Clear button ---
        if (ImGui::Button("Clear"))
        {
            m_logStorage->clear();
        }

        // --- Severity filter toggles (in a single line) ---
        ImGui::Text("Show:");
        ImGui::SameLine();
        ImGui::Checkbox("Trace", &m_showTrace);
        ImGui::SameLine();
        ImGui::Checkbox("Debug", &m_showDebug);
        ImGui::SameLine();
        ImGui::Checkbox("Info", &m_showInfo);
        ImGui::SameLine();
        ImGui::Checkbox("Warn", &m_showWarn);
        ImGui::SameLine();
        ImGui::Checkbox("Error", &m_showError);
        ImGui::SameLine();
        ImGui::Checkbox("Critical", &m_showCritical);

        ImGui::EndGroup();

        // Separator
        ImGui::Separator();

        // ------------------------------------------------------------
        // 2. Log entries (in a scrollable child region)
        // ------------------------------------------------------------
        const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("LogScrollRegion", ImVec2(0, -footerHeightToReserve), true);

        // We need to know if the user is currently at the bottom
        static bool wasAtBottom = true;
        if (m_autoScroll)
        {
            // Check if we are at the bottom before new entries are rendered
            wasAtBottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY();
        }

        // Iterate over all stored entries
        m_logStorage->forEach([this](const LogEntry& entry)
            {
                // --- Filter by severity ---
                const auto level = entry.getLevel();
                if (!isLevelVisible(level, m_showTrace, m_showDebug, m_showInfo,
                    m_showWarn, m_showError, m_showCritical))
                {
                    return;  // Skip this entry
                }

                // --- Filter by text (if filter is non-empty) ---
                if (m_filterBuffer[0] != '\0')
                {
                    const std::string formatted = entry.format();
                    const std::string filterLower = m_filterBuffer;
                    // Case-insensitive search
                    std::string textLower = formatted;
                    std::transform(textLower.begin(), textLower.end(), textLower.begin(), ::tolower);
                    if (textLower.find(filterLower) == std::string::npos)
                    {
                        return;  // Skip this entry
                    }
                }

                // --- Render the entry with color ---
                const ImVec4 color = getSeverityColor(level);
                ImGui::PushStyleColor(ImGuiCol_Text, color);

                // Format and display
                const std::string formatted = entry.format();
                ImGui::TextUnformatted(formatted.c_str());

                ImGui::PopStyleColor();
            });

        // Auto-scroll to bottom if we were at the bottom before or new entries arrived
        if (m_autoScroll && wasAtBottom)
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();  // LogScrollRegion

        // ------------------------------------------------------------
        // 3. Footer (optional: show log count, etc.)
        // ------------------------------------------------------------
        ImGui::Text("Total: %zu entries", m_logStorage->getSize());

        ImGui::End();  // Debug Console
    }

}