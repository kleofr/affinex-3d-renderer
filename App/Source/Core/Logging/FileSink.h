// FileSink.h
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
    // FileSink – forwards formatted logs to spdlog's file logger.
    // - Converts LogEntry to spdlog level and calls the logger.
    // - File rotation, formatting, and flushing are handled by spdlog.
    // - Target: File.
    // ------------------------------------------------------------------------
    class FileSink : public Sink
    {
    public:
        // Constructor – takes ownership of (or shares) the spdlog file logger.
        explicit FileSink(std::shared_ptr<spdlog::logger> fileLogger);

        // Sink interface implementations
        void log(const LogEntry& entry) override;
        LogTarget getTarget() const override;

    private:
        std::shared_ptr<spdlog::logger> m_logger;
    };

} // namespace AffineX