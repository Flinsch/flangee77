#ifndef XL7_GRAPHICS_SHADERS_INCLUDEHANDLER_H
#define XL7_GRAPHICS_SHADERS_INCLUDEHANDLER_H

#include <CoreLabs/string.h>



namespace xl7::graphics::shaders {



class IncludeHandler
{

public:
    virtual ~IncludeHandler() = default;



    /**
     * Extracts the directory part from the specified shader include path,
     * preserving any trailing delimiter. Returns an empty string if a separation
     * into directory and filename based on a delimiter is not possible.
     */
    virtual cl7::u8string directory(const cl7::u8string& path);

    /**
     * Extracts the filename part from the specified shader include path. Returns
     * the full path if a separation into directory and filename based on a
     * delimiter is not possible.
     */
    virtual cl7::u8string filename(const cl7::u8string& path);

    /**
     * Reads the high-level shader source code from the specified source path.
     * Returns the read source code on success, an empty string on failure.
     */
    virtual cl7::u8string read_source_code(const cl7::u8string& file_path) = 0;

}; // class IncludeHandler



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_INCLUDEHANDLER_H
