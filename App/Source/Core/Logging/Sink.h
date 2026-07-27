// Sink.h
#pragma once

#include "../Logger_Module.h"  // For LogLevel and LogTarget enums

namespace AffineX
{
    // Forward declaration to avoid including the full LogEntry header
    class LogEntry;

    // ------------------------------------------------------------------------
    // Abstract base class for all log sinks.
    // Each sink must:
    //   - implement log(const LogEntry&) to handle the entry
    //   - return its target type (Console, File, ImGui, or a combination)
    // ------------------------------------------------------------------------
    class Sink
    {
    public:
        virtual ~Sink() = default;

        // Process the log entry.
        // This will be called by Logger_Module when the sink's target matches.
        virtual void log(const LogEntry& entry) = 0;

        // Returns the bitmask of targets this sink serves (e.g., Console | File).
        // Used by Logger_Module to filter which sinks receive each log entry.
        virtual LogTarget getTarget() const = 0;
    };

} // namespace AffineX