// LogEntry.h
#pragma once

#include "../Logger_Module.h"  // For LogLevel enum
#include <string>
#include <chrono>
#include <cstdint>

namespace AffineX
{
    // ------------------------------------------------------------------------
    // Severity levels (mirrors spdlog's levels for easy conversion)
    // ------------------------------------------------------------------------
    //enum class LogLevel : uint8_t
    //{
    //    Trace,
    //    Debug,
    //    Info,
    //    Warn,
    //    Error,
    //    Critical
    //};

    // ------------------------------------------------------------------------
    // A single log entry – immutable, move‑only container
    // ------------------------------------------------------------------------
    class LogEntry
    {
    public:
        // Default constructor required for ring‑buffer pre‑allocation
        LogEntry() = default;

        // Full constructor – takes the raw data, stores it by value
        LogEntry(std::chrono::system_clock::time_point timestamp,
            LogLevel level,
            const char* file,
            int line,
            const char* function,
            std::string message);

        // Move‑only – copying would be expensive; ring buffer uses move semantics
        LogEntry(LogEntry&&) = default;
        LogEntry& operator=(LogEntry&&) = default;

        LogEntry(const LogEntry&) = delete;
        LogEntry& operator=(const LogEntry&) = delete;

        // ---- Getters ----
        std::chrono::system_clock::time_point getTimestamp() const { return m_timestamp; }
        LogLevel getLevel() const { return m_level; }
        const std::string& getFile() const { return m_file; }
        int getLine() const { return m_line; }
        const std::string& getFunction() const { return m_function; }
        const std::string& getMessage() const { return m_message; }

        // ---- Formatting ----
        // Produces a string like: "[2026-07-21 14:32:10.123] [Info] main.cpp:42 in main() -> Hello world"
        std::string format() const;

    private:
        std::chrono::system_clock::time_point m_timestamp;
        LogLevel m_level = LogLevel::Info;
        std::string m_file;
        int m_line = 0;
        std::string m_function;
        std::string m_message;
    };

} // namespace AffineX