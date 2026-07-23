// Logger_Module.cpp
#include "Logger_Module.h"

#include "Logging/LogEntry.h"
#include "Logging/LogStorage.h"
#include "Logging/Sink.h"
#include "Logging/ConsoleSink.h"
#include "Logging/FileSink.h"
#include "Logging/ImGuiSink.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <chrono>
#include <utility>

namespace AffineX
{

    // ------------------------------------------------------------------------
    // Static member initialisation
    // ------------------------------------------------------------------------
    Logger_Module* Logger_Module::s_instance = nullptr;

    // ------------------------------------------------------------------------
    // Constructor / Destructor
    // ------------------------------------------------------------------------
    Logger_Module::Logger_Module()
        : m_storage(std::make_unique<LogStorage>())
    {
        // Storage is ready, but spdlog and sinks are set up in init()
    }

    Logger_Module::~Logger_Module()
    {
        // Ensure shutdown was called explicitly by the Engine
        if (s_instance == this)
        {
            shutdown();
        }
    }

    // ------------------------------------------------------------------------
    // Initialisation
    // ------------------------------------------------------------------------
    void Logger_Module::init()
    {
        // 1. Set the singleton pointer so macros can find us
        s_instance = this;

        // 2. Create spdlog console logger (with colours)
        m_consoleLogger = spdlog::stdout_color_mt("console");
        m_consoleLogger->set_level(spdlog::level::trace);
        m_consoleLogger->flush_on(spdlog::level::trace);

        // 3. Create spdlog rotating file logger (5 MB per file, keep 3 backups)
        m_fileLogger = spdlog::rotating_logger_mt("file", "Log.txt", 1024 * 1024 * 5, 3);
        m_fileLogger->set_level(spdlog::level::trace);
        m_fileLogger->flush_on(spdlog::level::trace);

        // 4. Create our custom sink wrappers and register them
        m_sinks.clear();
        m_sinks.push_back(std::make_unique<ConsoleSink>(m_consoleLogger));
        m_sinks.push_back(std::make_unique<FileSink>(m_fileLogger));
        m_sinks.push_back(std::make_unique<ImGuiSink>(m_storage.get()));

        // 5. (Optional) Set a global pattern for spdlog – but our sinks will
        //    call entry.format() anyway, so this is just a fallback.
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    }

    // ------------------------------------------------------------------------
    // Shutdown
    // ------------------------------------------------------------------------
    void Logger_Module::shutdown()
    {
        // Flush all spdlog loggers to ensure everything is written
        spdlog::flush_all();

        // Drop spdlog loggers (releases resources)
        spdlog::drop("console");
        spdlog::drop("file");

        m_consoleLogger.reset();
        m_fileLogger.reset();

        // Clear custom sinks and storage
        m_sinks.clear();
        m_storage.reset();

        // Clear singleton pointer
        s_instance = nullptr;
    }

    // ------------------------------------------------------------------------
    // Add a custom sink (runtime registration)
    // ------------------------------------------------------------------------
    void Logger_Module::addSink(std::unique_ptr<Sink> sink)
    {
        if (sink)
        {
            m_sinks.push_back(std::move(sink));
        }
    }

    // ------------------------------------------------------------------------
    // Main logging entry point (called by macros)
    // ------------------------------------------------------------------------
    void Logger_Module::log(LogLevel level,
        LogTarget target,
        const char* file,
        int line,
        const char* function,
        const std::string& message)
    {
        // 1. Build a LogEntry with the current timestamp
        const auto now = std::chrono::system_clock::now();
        LogEntry entry(now, level, file, line, function, message);

        // 2. Store it permanently in the ring buffer (for ImGui history)
        if (m_storage)
        {
            m_storage->addEntry(entry);
        }

        // 3. Dispatch to the requested sink(s)
        dispatchToSinks(entry, target);
    }

    // ------------------------------------------------------------------------
    // Internal dispatch – only calls sinks that match the target bitmask
    // ------------------------------------------------------------------------
    void Logger_Module::dispatchToSinks(const LogEntry& entry, LogTarget target)
    {
        // If target is None, do nothing (useful for disabling logging)
        if (target == LogTarget::None)
            return;

        for (const auto& sink : m_sinks)
        {
            // Each sink knows its own target type (e.g., ConsoleSink returns LogTarget::Console)
            if ((static_cast<uint8_t>(target) & static_cast<uint8_t>(sink->getTarget())) != 0)
            {
                sink->log(entry);
            }
        }
    }

} // namespace AffineX