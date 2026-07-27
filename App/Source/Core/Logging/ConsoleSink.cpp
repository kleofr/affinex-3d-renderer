// ConsoleSink.cpp
#include "ConsoleSink.h"

#include "LogEntry.h"

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>

namespace AffineX
{

    // ------------------------------------------------------------------------
    // Constructor – stores the console logger
    // ------------------------------------------------------------------------
    ConsoleSink::ConsoleSink(std::shared_ptr<spdlog::logger> consoleLogger)
        : m_logger(std::move(consoleLogger))
    {
        // All done
    }

    // ------------------------------------------------------------------------
    // Convert LogLevel to spdlog level
    // ------------------------------------------------------------------------
    static spdlog::level::level_enum toSpdlogLevel(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Trace:    return spdlog::level::trace;
        case LogLevel::Debug:    return spdlog::level::debug;
        case LogLevel::Info:     return spdlog::level::info;
        case LogLevel::Warn:     return spdlog::level::warn;
        case LogLevel::Error:    return spdlog::level::err;
        case LogLevel::Critical: return spdlog::level::critical;
        default:                 return spdlog::level::info;
        }
    }

    // ------------------------------------------------------------------------
    // Forward the log entry to spdlog
    // ------------------------------------------------------------------------
    void ConsoleSink::log(const LogEntry& entry)
    {
        if (!m_logger)
            return;  // Safety – should not happen if initialised properly

        // Convert level
        const auto spdLevel = toSpdlogLevel(entry.getLevel());

        // Format the entry once
        const std::string formatted = entry.format();

        // Forward to spdlog – it will handle colouring and thread safety
        m_logger->log(spdLevel, formatted);
    }

    // ------------------------------------------------------------------------
    // Return the target this sink serves
    // ------------------------------------------------------------------------
    LogTarget ConsoleSink::getTarget() const
    {
        return LogTarget::Console;
    }

} // namespace AffineX