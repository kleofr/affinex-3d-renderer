// Logger_Module.h
#pragma once

#include <string>
#include <memory>
#include <vector>

// Forward declarations to avoid heavy includes in the header
namespace AffineX
{
    class LogEntry;
    class LogStorage;
    class Sink;
}

// Forward declare spdlog loggers (we only need pointers)
namespace spdlog
{
    class logger;
}

namespace AffineX
{

    // Severity levels (mirroring spdlog's levels for easy conversion)
    enum class LogLevel : uint8_t
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    // Bitmask targets – allows a macro to send to multiple sinks at once
    enum class LogTarget : uint8_t
    {
        None = 0,
        Console = 1 << 0,
        File = 1 << 1,
        All = Console | File
    };

    // Enable bitwise operations for LogTarget (so users can write Console | File)
    inline LogTarget operator|(LogTarget a, LogTarget b)
    {
        return static_cast<LogTarget>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }

    inline bool operator&(LogTarget a, LogTarget b)
    {
        return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
    }

    // ------------------------------------------------------------------------
    // Logger_Module – the central dispatcher
    // ------------------------------------------------------------------------
    class Logger_Module
    {
    public:
        Logger_Module();
        ~Logger_Module();

        // Called by Engine::InitEngine – sets up spdlog and default sinks
        void init();

        // Called by Engine shutdown – flushes spdlog and frees resources
        void shutdown();

        // Main entry point for all macros.
        // Captures file/line/function automatically via macro expansion.
        void log(LogLevel level,
            LogTarget target,
            const char* file,
            int line,
            const char* function,
            const std::string& message);

        // Register an additional custom sink (e.g., network sink) at runtime
        void addSink(std::unique_ptr<Sink> sink);

        // Static accessor – allows macros to call without an Engine reference
        static Logger_Module* get() { return s_instance; }
		static LogStorage* getStorage() { return s_instance ? s_instance->m_storage.get() : nullptr; }

        // Disable copy/move (singleton-like behaviour, owned by Engine)
        Logger_Module(const Logger_Module&) = delete;
        Logger_Module& operator=(const Logger_Module&) = delete;
        Logger_Module(Logger_Module&&) = delete;
        Logger_Module& operator=(Logger_Module&&) = delete;

    private:
        // Internal helpers
        void dispatchToSinks(const LogEntry& entry, LogTarget target);

        // Storage for history (used by ImGuiSink)
        std::unique_ptr<LogStorage> m_storage;

        // All registered sinks (Console, File, custom)
        std::vector<std::unique_ptr<Sink>> m_sinks;

        // spdlog logger instances – owned by this module
        std::shared_ptr<spdlog::logger> m_consoleLogger;
        std::shared_ptr<spdlog::logger> m_fileLogger;

        // Singleton pointer – set in init(), cleared in shutdown()
        static Logger_Module* s_instance;
    };

} // namespace AffineX