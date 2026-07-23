#pragma once

#include "Logger_Module.h"

#define AX_TRACE(...)    ::AffineX::Logger_Module::GetLogger()->trace(__VA_ARGS__)
#define AX_INFO(...)     ::AffineX::Logger_Module::GetLogger()->info(__VA_ARGS__)
#define AX_WARN(...)     ::AffineX::Logger_Module::GetLogger()->warn(__VA_ARGS__)
#define AX_ERROR(...)    ::AffineX::Logger_Module::GetLogger()->error(__VA_ARGS__)
#define AX_CRITICAL(...) ::AffineX::Logger_Module::GetLogger()->critical(__VA_ARGS__)