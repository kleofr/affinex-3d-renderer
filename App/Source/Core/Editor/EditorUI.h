// ============================================================================
// EditorUI.h – Public interface for the editor panel library.
// ============================================================================
// This header declares all functions used to render ImGui panels.
// It does not depend on engine internals except for a forward declaration
// of LogStorage. All drawing is done via ImGui.
// ============================================================================

#pragma once

#include <imgui.h>
#include <cstddef>  // for size_t
#include "../Logging/LogStorage.h"

// Forward declaration: the UI library only needs a pointer to this.

namespace AffineX {

    // ------------------------------------------------------------------------
    // Console filter state – owned by the host, passed by reference.
    // ------------------------------------------------------------------------
    //struct ConsoleFilterState {
    //    bool showTrace = true;
    //    bool showDebug = true;
    //    bool showInfo = true;
    //    bool showWarn = true;
    //    bool showError = true;
    //    bool showCritical = true;
    //    bool autoScroll = true;
    //};

    // ------------------------------------------------------------------------
    // Styling
    // ------------------------------------------------------------------------
    // Applies a dark, Unreal‑inspired theme (flat colours, zero rounding).
    void ApplyStyle();

    // ------------------------------------------------------------------------
    // Main editor panels
    // ------------------------------------------------------------------------

    // The central 3D viewport. Draws a placeholder image and overlay stats.
    void RenderViewport();

    // World Outliner – hierarchy of actors/objects.
    void RenderOutliner();

    // Details panel – property grid for the selected object.
    void RenderDetails();

    // ------------------------------------------------------------------------
    // Debug Console
    // ------------------------------------------------------------------------
    // Renders the log viewer. All filter state is passed in and modified here.
    // Parameters:
    //   storage         – pointer to the log storage (must not be null).
    //   filterBuffer    – mutable character array for the text filter.
    //   filterBufSize   – size of that array.
    //   filterState     – all toggles and auto‑scroll flag.
    void RenderConsole(AffineX::LogStorage* storage,
        char* filterBuffer,
        size_t filterBufSize);

} // namespace AffineX