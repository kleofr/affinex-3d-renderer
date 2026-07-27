#include "ShaderFileReader.h"

#include <fstream>
#include <filesystem>

namespace AffineX {

    std::string read_file(const std::filesystem::path& path) {
        try {
            // 1. Open the file in binary mode
            std::ifstream file(path, std::ios::binary);
            if (!file.is_open()) {
				LOG_ERROR("Failed to open file: {} ", path.string());
                return {};
            }

            // 2. Get file size and reserve exact capacity
            std::size_t file_size = std::filesystem::file_size(path);
            if (file_size == 0) {
                // Empty file – return empty string (no error, just empty)
                return {};
            }

            std::string content;
            content.resize(file_size);

            // 3. Read the entire file into the string buffer
            file.read(content.data(), static_cast<std::streamsize>(file_size));
            if (file.fail() || file.gcount() != static_cast<std::streamsize>(file_size)) {
                LOG_ERROR("Failed to read file completely: {}", path.string());
                return {};
            }

            // 4. Verify we read the expected number of bytes
            if (content.size() != file_size) {
				LOG_ERROR("Read size mismatch for file: {}", path.string());
                return {};
            }

            return content;

        }
        catch (const std::exception& e) {
			LOG_ERROR("Exception while reading file {}: {}", path.string(), e.what());
            return {};
        }
        catch (...) {
			LOG_ERROR("Unknown exception while reading file: {}", path.string());
            return {};
        }
    }

    std::string load_stage(const std::filesystem::path& base_path, ShaderStage stage) {
        // Build the full path by appending the appropriate extension
        std::filesystem::path full_path = base_path;

        switch (stage) {
        case ShaderStage::vert:
            full_path += ".vert";
            break;
        case ShaderStage::frag:
            full_path += ".frag";
            break;
            // Future stages (uncomment and add when ready):
            // case ShaderStage::geom: full_path += ".geom"; break;
            // case ShaderStage::tesc: full_path += ".tesc"; break;
            // case ShaderStage::tese: full_path += ".tese"; break;
            // case ShaderStage::comp: full_path += ".comp"; break;
        default:
			LOG_ERROR("Unknown shader stage requested for base path: {}", base_path.string());
            return {};
        }

        return read_file(full_path);
    }

} // namespace AffineX