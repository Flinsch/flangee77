#include "FileIncludeHandler.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/text/codec.h>

#include <filesystem>
#include <fstream>



namespace xl7::graphics::shaders {



    /**
     * Reads the high-level shader source code from the specified source path.
     * Returns the read source code on success, an empty string on failure.
     */
    cl7::u8string FileIncludeHandler::read_source_code(const cl7::u8string& file_path)
    {
        std::ifstream file(std::filesystem::path(cl7::text::codec::reinterpret_utf8(file_path)), std::ios::in | std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            LOG_ERROR(u8"Shader file \"" + file_path + u8"\" could not be opened. Does it exist?");
            return {};
        }

        const auto size = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        cl7::u8string code(size, 0);
        if (!file.read(reinterpret_cast<char*>(code.data()), static_cast<std::streamsize>(size)))
        {
            LOG_ERROR(u8"Shader file \"" + file_path + u8"\" could not be read.");
            return {};
        }

        return code;
    }



} // namespace xl7::graphics::shaders
