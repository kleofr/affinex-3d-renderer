#pragma once

#include <string>
#include <filesystem>
#include <Core/Logging/Log.h>

namespace AffineX {

    enum class ShaderStage {
        vert,
        frag
        // Future: geom, tesc, tese, comp
    };

    // Raw utility: reads any file into a string.
    // Returns empty string on failure and logs an error.
    std::string read_file(const std::filesystem::path& path);

    // Convenience wrapper: appends the appropriate extension based on stage.
    // Returns empty string on failure (forwards logging from read_file).
    std::string load_stage(const std::filesystem::path& base_path, ShaderStage stage);

} // namespace AffineX