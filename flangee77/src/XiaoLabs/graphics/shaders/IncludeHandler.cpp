#include "IncludeHandler.h"



namespace xl7::graphics::shaders {



    /**
     * Extracts the directory part from the specified shader include path,
     * preserving any trailing delimiter. Returns an empty string if a separation
     * into directory and filename based on a delimiter is not possible.
     */
    cl7::u8string IncludeHandler::directory(const cl7::u8string& path)
    {
        const size_t pos = path.find_last_of(u8"/\\");
        if (pos != cl7::u8string::npos)
            return path.substr(0, pos + 1);
        return {};
    }

    /**
     * Extracts the filename part from the specified shader include path. Returns
     * the full path if a separation into directory and filename based on a
     * delimiter is not possible.
     */
    cl7::u8string IncludeHandler::filename(const cl7::u8string& path)
    {
        const size_t pos = path.find_last_of(u8"/\\");
        if (pos != cl7::u8string::npos)
            return path.substr(pos + 1);
        return path;
    }



} // namespace xl7::graphics::shaders
