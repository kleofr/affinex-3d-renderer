// ConsoleSink.h
#pragma once

#include "Sink.h"

#include <memory>

namespace spdlog
{
    class logger;
}

namespace AffineX
{

    // ------------------------------------------------------------------------
    // ConsoleSink – forwards formatted logs to spdlog's console logger.
    // - Converts LogEntry to spdlog level and calls the logger.
    // - The actual output (colour, formatting) is handled by spdlog.
    // - Target: Console.
    // ------------------------------------------------------------------------
    class ConsoleSink : public Sink
    {
    public:
        // Constructor – takes ownership of (or shares) the spdlog console logger.
        explicit ConsoleSink(std::shared_ptr<spdlog::logger> consoleLogger);

        // Sink interface implementations
        void log(const LogEntry& entry) override;
        LogTarget getTarget() const override;

    private:
        std::shared_ptr<spdlog::logger> m_logger;
    };

} // namespace AffineX