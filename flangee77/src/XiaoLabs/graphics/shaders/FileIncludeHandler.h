#ifndef XL7_GRAPHICS_SHADERS_FILEINCLUDEHANDLER_H
#define XL7_GRAPHICS_SHADERS_FILEINCLUDEHANDLER_H
#include "./IncludeHandler.h"



namespace xl7::graphics::shaders {



class FileIncludeHandler
    : public IncludeHandler
{

public:
    ~FileIncludeHandler() override = default;



    /**
     * Reads the high-level shader source code from the specified source path.
     * Returns the read source code on success, an empty string on failure.
     */
    cl7::u8string read_source_code(const cl7::u8string& file_path) override;

}; // class FileIncludeHandler



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_FILEINCLUDEHANDLER_H
