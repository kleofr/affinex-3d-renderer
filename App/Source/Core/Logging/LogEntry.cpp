// LogEntry.cpp
#include "LogEntry.h"

#include <sstream>
#include <iomanip>
#include <ctime>

namespace AffineX
{

    // ------------------------------------------------------------------------
    // Constructor
    // ------------------------------------------------------------------------
    LogEntry::LogEntry(std::chrono::system_clock::time_point timestamp,
        LogLevel level,
        const char* file,
        int line,
        const char* function,
        std::string message)
        : m_timestamp(timestamp)
        , m_level(level)
        , m_file(file ? file : "")
        , m_line(line)
        , m_function(function ? function : "")
        , m_message(std::move(message))
    {
        // All initialisation is done in the initialiser list
    }

    // ------------------------------------------------------------------------
    // Convert LogLevel to a human‑readable string
    // ------------------------------------------------------------------------
    static const char* levelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Trace:    return "Trace";
        case LogLevel::Debug:    return "Debug";
        case LogLevel::Info:     return "Info";
        case LogLevel::Warn:     return "Warn";
        case LogLevel::Error:    return "Error";
        case LogLevel::Critical: return "Critical";
        default:                 return "Unknown";
        }
    }

    // ------------------------------------------------------------------------
    // Format the log entry as a single human‑readable string
    // ------------------------------------------------------------------------
    std::string LogEntry::format() const
    {
        // Convert timestamp to time_t (seconds since epoch)
        const auto time_t_epoch = std::chrono::system_clock::to_time_t(m_timestamp);

        // Get milliseconds part for sub‑second precision
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            m_timestamp.time_since_epoch()) % 1000;

        // Convert to local time (or UTC – choose what fits your needs)
        std::tm localTime;
        #if defined(_WIN32)
                localtime_s(&localTime, &time_t_epoch);
        #else
                localtime_r(&time_t_epoch, &localTime);
        #endif

        // Build the formatted string with a std::ostringstream
        std::ostringstream oss;

        // Timestamp: [YYYY-MM-DD HH:MM:SS.mmm]
        oss << std::put_time(&localTime, "[%Y-%m-%d %H:%M:%S");
        oss << '.' << std::setw(3) << std::setfill('0') << ms.count() << "] ";

        // Log level
        oss << '[' << levelToString(m_level) << "] ";

        // File:Line
        oss << m_file << ':' << m_line << ' ';

        // Function (if available)
        if (!m_function.empty())
        {
            oss << "in " << m_function << ' ';
        }

        // Arrow separator and the actual message
        oss << "-> " << m_message;

        return oss.str();
    }

} // namespace AffineX