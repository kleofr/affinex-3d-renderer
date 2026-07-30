// ============================================================================
// AffineX.cpp – Implementation of the editor panel library.
// ============================================================================
// All drawing functions are pure ImGui – no engine coupling.
// ============================================================================

#include "EditorUI.h"


#include <algorithm>
#include <cctype>
#include <string>

// ------------------------------------------------------------
// Helper functions (originally static in GUI_Module.cpp)
// ------------------------------------------------------------
namespace AffineX {

    // Convert LogLevel to ImGui color.
    ImVec4 GetSeverityColor(LogLevel level) {
        switch (level) {
        case LogLevel::Trace:    return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);   // Grey
        case LogLevel::Debug:    return ImVec4(0.3f, 0.7f, 0.9f, 1.0f);   // Light Blue
        case LogLevel::Info:     return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // White
        case LogLevel::Warn:     return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);   // Yellow
        case LogLevel::Error:    return ImVec4(1.0f, 0.4f, 0.2f, 1.0f);   // Orange-Red
        case LogLevel::Critical: return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);   // Bright Red
        default:                 return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }

    // Check if a severity level is visible based on filter flags.
    bool IsLevelVisible(LogLevel level,
        bool showTrace, bool showDebug, bool showInfo,
        bool showWarn, bool showError, bool showCritical) {
        switch (level) {
        case LogLevel::Trace:    return showTrace;
        case LogLevel::Debug:    return showDebug;
        case LogLevel::Info:     return showInfo;
        case LogLevel::Warn:     return showWarn;
        case LogLevel::Error:    return showError;
        case LogLevel::Critical: return showCritical;
        default:                 return true;
        }
    }

} // anonymous namespace

// ------------------------------------------------------------
// Styling
// ------------------------------------------------------------
void AffineX::ApplyStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Dark, sharp, Unreal‑inspired colours.
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.27f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.35f, 0.35f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.47f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.47f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.15f, 0.60f, 0.95f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.27f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.47f, 0.83f, 0.50f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.47f, 0.83f, 0.70f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.47f, 0.83f, 0.90f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.47f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.27f, 0.27f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.47f, 0.83f, 0.70f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.00f, 0.47f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 0.47f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 0.47f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.15f, 0.60f, 0.95f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.47f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.15f, 0.60f, 0.95f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.47f, 0.83f, 0.30f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 0.47f, 0.83f, 0.80f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 0.47f, 0.83f, 0.80f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.00f, 0.47f, 0.83f, 0.80f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);

    // Sharp corners – no rounding.
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;

    // Padding and spacing tuned for a professional look.
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.FramePadding = ImVec2(4.0f, 4.0f);
    style.ItemSpacing = ImVec2(6.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.IndentSpacing = 10.0f;
    style.ScrollbarSize = 12.0f;
    style.GrabMinSize = 8.0f;
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;
    style.TabBarBorderSize = 1.0f;
}

// ------------------------------------------------------------
// Viewport
// ------------------------------------------------------------
void AffineX::RenderViewport() {
    ImGui::Begin("Viewport");

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 min = ImGui::GetCursorScreenPos();
    ImVec2 max = ImVec2(min.x + viewportSize.x, min.y + viewportSize.y);

    // Dark background placeholder.
    draw->AddRectFilled(min, max, IM_COL32(30, 30, 35, 255));

    // Simple text in the center to indicate it's the render target.
    const char* placeholderText = "Render Viewport\n(Your 3D render target goes here)";
    ImVec2 textSize = ImGui::CalcTextSize(placeholderText);
    ImVec2 textPos = ImVec2(min.x + (viewportSize.x - textSize.x) * 0.5f,
        min.y + (viewportSize.y - textSize.y) * 0.5f);
    draw->AddText(textPos, IM_COL32(200, 200, 200, 255), placeholderText);

    // Overlay stats in the top‑left corner.
    ImVec2 statsPos = ImVec2(min.x + 10.0f, min.y + 10.0f);
    draw->AddText(statsPos, IM_COL32(255, 255, 0, 200),
        "FPS: 60 | Draws: 1024 | Tris: 2.1M");

    // (Optional) If you want to capture mouse when hovering, you can check
    // ImGui::IsWindowHovered() and conditionally set io.WantCaptureMouse = false.
    // That logic belongs in the engine host, not here.

    ImGui::End();
}

// ------------------------------------------------------------
// Outliner
// ------------------------------------------------------------
void AffineX::RenderOutliner() {
    ImGui::Begin("Outliner");

    static char filterBuffer[64] = "";
    ImGui::InputTextWithHint("##OutlinerFilter", "Search actors...",
        filterBuffer, sizeof(filterBuffer));

    // Dummy hierarchy.
    static bool rootOpen = true;
    if (ImGui::TreeNodeEx("Scene Root", ImGuiTreeNodeFlags_DefaultOpen)) {
        static const char* actors[] = {
            "DirectionalLight",
            "SkySphere",
            "Floor_01",
            "StaticMesh_Cube",
            "StaticMesh_Sphere",
            "PlayerStart"
        };
        for (const char* actor : actors) {
            ImGui::TreeNodeEx(actor,
                ImGuiTreeNodeFlags_Leaf |
                ImGuiTreeNodeFlags_NoTreePushOnOpen |
                ImGuiTreeNodeFlags_SpanAvailWidth);
            // In a real implementation, clicking would set a selected ID.
        }
        ImGui::TreePop();
    }

    ImGui::End();
}

// ------------------------------------------------------------
// Details (Property Grid)
// ------------------------------------------------------------
void AffineX::RenderDetails() {
    ImGui::Begin("Details");

    ImGui::Text("Selected: StaticMesh_Cube");
    ImGui::Separator();

    static float location[3] = { 0.0f, 1.5f, 0.0f };
    static float rotation[3] = { 0.0f, 45.0f, 0.0f };
    static float scale[3] = { 1.0f, 1.0f, 1.0f };
	static float baseColor[3] = { 0.8f, 0.2f, 0.1f };

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::BeginTable("TransformTable", 2,
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_NoSavedSettings);
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

        // Location
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Location");
        ImGui::TableSetColumnIndex(1);
        ImGui::DragFloat3("##Location", location, 0.1f);

        // Rotation
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Rotation");
        ImGui::TableSetColumnIndex(1);
        ImGui::DragFloat3("##Rotation", rotation, 0.5f);

        // Scale
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Scale");
        ImGui::TableSetColumnIndex(1);
        ImGui::DragFloat3("##Scale", scale, 0.1f, 0.01f, 100.0f);

        ImGui::EndTable();
    }

    if (ImGui::CollapsingHeader("Material")) {
        ImGui::Text("Base Color");
        ImGui::ColorEdit3("##BaseColor", baseColor,
            ImGuiColorEditFlags_NoInputs);

        ImGui::Text("Metallic");
        float metallic = 0.0f;
        ImGui::SliderFloat("##Metallic", &metallic, 0.0f, 1.0f);

        ImGui::Text("Roughness");
        float roughness = 0.5f;
        ImGui::SliderFloat("##Roughness", &roughness, 0.0f, 1.0f);
    }

    ImGui::End();
}


// ------------------------------------------------------------
// Debug Console (moved from GUI_Module.cpp)
// ------------------------------------------------------------
void AffineX::RenderConsole(AffineX::LogStorage* storage,
    char* filterBuffer,
    size_t filterBufSize)
{
    if (!storage)
        return;

    // Persistent UI state – kept inside this function.
    static bool showTrace = true;
    static bool showDebug = true;
    static bool showInfo = true;
    static bool showWarn = true;
    static bool showError = true;
    static bool showCritical = true;
    static bool autoScroll = true;

    ImGui::Begin("Debug Console");

    // ------------------------------------------------------------
    // Control bar
    // ------------------------------------------------------------
    ImGui::BeginGroup();

    ImGui::InputText("Filter", filterBuffer, filterBufSize);
    ImGui::SameLine();

    ImGui::Checkbox("Auto-scroll", &autoScroll);
    ImGui::SameLine();

    if (ImGui::Button("Clear")) {
        storage->clear();
    }

    ImGui::Text("Show:");
    ImGui::SameLine();
    ImGui::Checkbox("Trace", &showTrace);
    ImGui::SameLine();
    ImGui::Checkbox("Debug", &showDebug);
    ImGui::SameLine();
    ImGui::Checkbox("Info", &showInfo);
    ImGui::SameLine();
    ImGui::Checkbox("Warn", &showWarn);
    ImGui::SameLine();
    ImGui::Checkbox("Error", &showError);
    ImGui::SameLine();
    ImGui::Checkbox("Critical", &showCritical);

    ImGui::EndGroup();
    ImGui::Separator();

    // ------------------------------------------------------------
    // Scrollable log area
    // ------------------------------------------------------------
    const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y +
        ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("LogScrollRegion", ImVec2(0, -footerHeightToReserve), true);

    static bool wasAtBottom = true;
    if (autoScroll) {
        wasAtBottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY();
    }

    // Helper to convert LogLevel to a display string.
    auto levelToString = [](LogLevel level) -> const char* {
        switch (level) {
        case LogLevel::Trace:    return "Trace";
        case LogLevel::Debug:    return "Debug";
        case LogLevel::Info:     return "Info";
        case LogLevel::Warn:     return "Warn";
        case LogLevel::Error:    return "Error";
        case LogLevel::Critical: return "Critical";
        default:                 return "Unknown";
        }
        };

    storage->forEach([&](const AffineX::LogEntry& entry) {
        LogLevel level = entry.getLevel();

        // Severity filter.
        if (!IsLevelVisible(level,
            showTrace, showDebug, showInfo,
            showWarn, showError, showCritical)) {
            return;
        }

        // Get the full formatted string.
        std::string fullText = entry.format();

        // Text filter (case‑insensitive).
        if (filterBuffer[0] != '\0') {
            std::string filterLower = filterBuffer;
            std::transform(filterLower.begin(), filterLower.end(),
                filterLower.begin(), ::tolower);
            std::string textLower = fullText;
            std::transform(textLower.begin(), textLower.end(),
                textLower.begin(), ::tolower);
            if (textLower.find(filterLower) == std::string::npos) {
                return;
            }
        }

        // ----- Split location and message -----
        // Format: "path\to\file.cpp:line in Function -> message"
        size_t splitPos = fullText.find(" -> ");
        std::string locationPart;
        std::string messagePart;

        if (splitPos != std::string::npos) {
            locationPart = fullText.substr(0, splitPos);
            messagePart = fullText.substr(splitPos + 4); // skip " -> "
        }
        else {
            locationPart = "";
            messagePart = fullText;
        }

        // Extract just the filename (e.g., "Engine.cpp:18")
        std::string shortLocation = locationPart;
        if (!locationPart.empty()) {
            size_t lastSlash = locationPart.find_last_of("\\/");
            if (lastSlash != std::string::npos) {
                shortLocation = locationPart.substr(lastSlash + 1);
            }
        }

        // Get the severity colour.
        ImVec4 color = GetSeverityColor(level);

        // ----- Render everything on ONE line with hanging indent for wrapped text -----
        ImGui::PushStyleColor(ImGuiCol_Text, color);

        // 1. Short location (grey, muted) with tooltip for full path
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "[%s]", shortLocation.c_str());
        if (ImGui::IsItemHovered() && !locationPart.empty()) {
            ImGui::BeginTooltip();
            ImGui::Text("Full location:");
            ImGui::TextUnformatted(locationPart.c_str());
            ImGui::EndTooltip();
        }

        // 2. Severity level (in the severity colour)
        ImGui::SameLine(0.0f, 6.0f);
        ImGui::TextColored(color, "%s", levelToString(level));

        // 3. Actual message – we want it to start after the level, and any wrapped
        //    lines should align with the start of the message (hanging indent).
        //    We achieve this by setting the cursor position to where the message begins,
        //    and then enabling word wrapping.
        ImGui::SameLine(0.0f, 6.0f);
        // Get the current cursor X position (this is the start of the message area)
        float messageStartX = ImGui::GetCursorPosX();
        // Set the cursor to that X (it already is, but we keep it for clarity)
        ImGui::SetCursorPosX(messageStartX);
        // Now draw the message with wrapping.
        ImGui::PushTextWrapPos(0.0f); // wrap at the right edge of the window
        ImGui::TextUnformatted(messagePart.c_str());
        ImGui::PopTextWrapPos();

        // IMPORTANT: After the message, the cursor will be at the end of the message,
        // which might be on a new line if it wrapped. We don't need to do anything else.

        ImGui::PopStyleColor(); // restore text colour
        });

    if (autoScroll && wasAtBottom) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    // ------------------------------------------------------------
    // Footer
    // ------------------------------------------------------------
    ImGui::Text("Total: %zu entries", storage->getSize());

    ImGui::End();
}