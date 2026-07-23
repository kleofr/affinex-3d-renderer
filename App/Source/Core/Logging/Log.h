// Log.h
#pragma once

#include "../Logger_Module.h"

// ------------------------------------------------------------------------
// Compile-time Log Level Filtering
// - In Debug builds, everything from Trace upwards is compiled in.
// - In Release builds, Debug and Trace are stripped out (Info and above remain).
// - Override by defining AFX_ACTIVE_LOG_LEVEL before including this header.
// ------------------------------------------------------------------------
#ifndef AFX_ACTIVE_LOG_LEVEL
#ifdef _DEBUG
#define AFX_ACTIVE_LOG_LEVEL AffineX::LogLevel::Trace
#else
#define AFX_ACTIVE_LOG_LEVEL AffineX::LogLevel::Info
#endif
#endif

// ------------------------------------------------------------------------
// Core Logging Macro
// ------------------------------------------------------------------------
#define AFX_LOG(level, target, msg)                                                                        \
    do                                                                                                     \
    {                                                                                                      \
        if (static_cast<uint8_t>(level) >= static_cast<uint8_t>(AFX_ACTIVE_LOG_LEVEL))                     \
        {                                                                                                  \
            if (auto* logger = AffineX::Logger_Module::get())                                              \
            {                                                                                              \
                logger->log(level, target, __FILE__, __LINE__, __FUNCTION__, msg);                         \
            }                                                                                              \
        }                                                                                                  \
    } while (0)

// ------------------------------------------------------------------------
// Generic Log Macro (manual target combination)
// Example: LOG(AffineX::LogLevel::Info, AffineX::LogTarget::Console | AffineX::LogTarget::File, "Hello");
// ------------------------------------------------------------------------
#define LOG(level, target, msg) AFX_LOG(level, target, msg)

// ========================================================================
// Convenience Shorthands
// ========================================================================

// ---------- All Sinks (Console + File + ImGui) ----------
#define LOG_TRACE(msg)    AFX_LOG(AffineX::LogLevel::Trace,    AffineX::LogTarget::All, msg)
#define LOG_DEBUG(msg)    AFX_LOG(AffineX::LogLevel::Debug,    AffineX::LogTarget::All, msg)
#define LOG_INFO(msg)     AFX_LOG(AffineX::LogLevel::Info,     AffineX::LogTarget::All, msg)
#define LOG_WARN(msg)     AFX_LOG(AffineX::LogLevel::Warn,     AffineX::LogTarget::All, msg)
#define LOG_ERROR(msg)    AFX_LOG(AffineX::LogLevel::Error,    AffineX::LogTarget::All, msg)
#define LOG_CRITICAL(msg) AFX_LOG(AffineX::LogLevel::Critical, AffineX::LogTarget::All, msg)

// ---------- Console Only ----------
#define LOG_TRACE_CONSOLE(msg)    AFX_LOG(AffineX::LogLevel::Trace,    AffineX::LogTarget::Console, msg)
#define LOG_DEBUG_CONSOLE(msg)    AFX_LOG(AffineX::LogLevel::Debug,    AffineX::LogTarget::Console, msg)
#define LOG_INFO_CONSOLE(msg)     AFX_LOG(AffineX::LogLevel::Info,     AffineX::LogTarget::Console, msg)
#define LOG_WARN_CONSOLE(msg)     AFX_LOG(AffineX::LogLevel::Warn,     AffineX::LogTarget::Console, msg)
#define LOG_ERROR_CONSOLE(msg)    AFX_LOG(AffineX::LogLevel::Error,    AffineX::LogTarget::Console, msg)
#define LOG_CRITICAL_CONSOLE(msg) AFX_LOG(AffineX::LogLevel::Critical, AffineX::LogTarget::Console, msg)

// ---------- File Only ----------
#define LOG_TRACE_FILE(msg)    AFX_LOG(AffineX::LogLevel::Trace,    AffineX::LogTarget::File, msg)
#define LOG_DEBUG_FILE(msg)    AFX_LOG(AffineX::LogLevel::Debug,    AffineX::LogTarget::File, msg)
#define LOG_INFO_FILE(msg)     AFX_LOG(AffineX::LogLevel::Info,     AffineX::LogTarget::File, msg)
#define LOG_WARN_FILE(msg)     AFX_LOG(AffineX::LogLevel::Warn,     AffineX::LogTarget::File, msg)
#define LOG_ERROR_FILE(msg)    AFX_LOG(AffineX::LogLevel::Error,    AffineX::LogTarget::File, msg)
#define LOG_CRITICAL_FILE(msg) AFX_LOG(AffineX::LogLevel::Critical, AffineX::LogTarget::File, msg)

// ---------- ImGui Panel Only ----------
#define LOG_TRACE_IMGUI(msg)    AFX_LOG(AffineX::LogLevel::Trace,    AffineX::LogTarget::ImGui, msg)
#define LOG_DEBUG_IMGUI(msg)    AFX_LOG(AffineX::LogLevel::Debug,    AffineX::LogTarget::ImGui, msg)
#define LOG_INFO_IMGUI(msg)     AFX_LOG(AffineX::LogLevel::Info,     AffineX::LogTarget::ImGui, msg)
#define LOG_WARN_IMGUI(msg)     AFX_LOG(AffineX::LogLevel::Warn,     AffineX::LogTarget::ImGui, msg)
#define LOG_ERROR_IMGUI(msg)    AFX_LOG(AffineX::LogLevel::Error,    AffineX::LogTarget::ImGui, msg)
#define LOG_CRITICAL_IMGUI(msg) AFX_LOG(AffineX::LogLevel::Critical, AffineX::LogTarget::ImGui, msg)