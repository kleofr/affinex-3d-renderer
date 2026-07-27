// Log.h
#pragma once

#include "../Logger_Module.h"
#include <spdlog/fmt/fmt.h>   // or <fmt/format.h> if you have it separately

// ------------------------------------------------------------------------
// Compile-time Log Level Filtering (unchanged)
// ------------------------------------------------------------------------
#ifndef AFX_ACTIVE_LOG_LEVEL
#ifdef _DEBUG
#define AFX_ACTIVE_LOG_LEVEL AffineX::LogLevel::Trace
#else
#define AFX_ACTIVE_LOG_LEVEL AffineX::LogLevel::Info
#endif
#endif

// ------------------------------------------------------------------------
// Core logging macro with formatting
// - Uses fmt::format to create a single string from the format string and args.
// - Then calls Logger_Module::log() with that string.
// ------------------------------------------------------------------------
#define AFX_LOG_FORMAT(level, target, format_str, ...)                                                         \
    do                                                                                                  \
    {                                                                                                   \
        if (static_cast<uint8_t>(level) >= static_cast<uint8_t>(AFX_ACTIVE_LOG_LEVEL))                 \
        {                                                                                               \
            if (auto* logger = AffineX::Logger_Module::get())                                          \
            {                                                                                           \
                std::string formattedMsg = fmt::format(format_str, ##__VA_ARGS__);                       \
                logger->log(level, target, __FILE__, __LINE__, __FUNCTION__, formattedMsg);             \
            }                                                                                           \
        }                                                                                               \
    } while (0)

// ------------------------------------------------------------------------
// Variadic convenience macros
// ------------------------------------------------------------------------
#define LOG(level, target, fmt, ...)    AFX_LOG_FORMAT(level, target, fmt, ##__VA_ARGS__)

// ---------- All Sinks (Console + File) ----------
#define LOG_TRACE(fmt, ...)    AFX_LOG_FORMAT(AffineX::LogLevel::Trace,    AffineX::LogTarget::All, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)    AFX_LOG_FORMAT(AffineX::LogLevel::Debug,    AffineX::LogTarget::All, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)     AFX_LOG_FORMAT(AffineX::LogLevel::Info,     AffineX::LogTarget::All, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)     AFX_LOG_FORMAT(AffineX::LogLevel::Warn,     AffineX::LogTarget::All, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)    AFX_LOG_FORMAT(AffineX::LogLevel::Error,    AffineX::LogTarget::All, fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) AFX_LOG_FORMAT(AffineX::LogLevel::Critical, AffineX::LogTarget::All, fmt, ##__VA_ARGS__)

// ---------- Console Only ----------
#define LOG_TRACE_CONSOLE(fmt, ...)    AFX_LOG_FORMAT(AffineX::LogLevel::Trace,    AffineX::LogTarget::Console, fmt, ##__VA_ARGS__)
#define LOG_DEBUG_CONSOLE(fmt, ...)    AFX_LOG_FORMAT(AffineX::LogLevel::Debug,    AffineX::LogTarget::Console, fmt, ##__VA_ARGS__)
#define LOG_INFO_CONSOLE(fmt, ...)     AFX_LOG_FORMAT(AffineX::LogLevel::Info,     AffineX::LogTarget::Console, fmt, ##__VA_ARGS__)
#define LOG_WARN_CONSOLE(fmt, ...)     AFX_LOG_FORMAT(AffineX::LogLevel::Warn,     AffineX::LogTarget::Console, fmt, ##__VA_ARGS__)
#define LOG_ERROR_CONSOLE(fmt, ...)    AFX_LOG_FORMAT(AffineX::LogLevel::Error,    AffineX::LogTarget::Console, fmt, ##__VA_ARGS__)
#define LOG_CRITICAL_CONSOLE(fmt, ...) AFX_LOG_FORMAT(AffineX::LogLevel::Critical, AffineX::LogTarget::Console, fmt, ##__VA_ARGS__)

// ---------- File Only ----------
#define LOG_TRACE_FILE(fmt, ...)    AFX_LOG_FORMAT(AffineX::LogLevel::Trace,    AffineX::LogTarget::File, fmt, ##__VA_ARGS__)
#define LOG_DEBUG_FILE(fmt, ...)    AFX_LOG_FORMAT(AffineX::LogLevel::Debug,    AffineX::LogTarget::File, fmt, ##__VA_ARGS__)
#define LOG_INFO_FILE(fmt, ...)     AFX_LOG_FORMAT(AffineX::LogLevel::Info,     AffineX::LogTarget::File, fmt, ##__VA_ARGS__)
#define LOG_WARN_FILE(fmt, ...)     AFX_LOG_FORMAT(AffineX::LogLevel::Warn,     AffineX::LogTarget::File, fmt, ##__VA_ARGS__)
#define LOG_ERROR_FILE(fmt, ...)    AFX_LOG_FORMAT(AffineX::LogLevel::Error,    AffineX::LogTarget::File, fmt, ##__VA_ARGS__)
#define LOG_CRITICAL_FILE(fmt, ...) AFX_LOG_FORMAT(AffineX::LogLevel::Critical, AffineX::LogTarget::File, fmt, ##__VA_ARGS__)