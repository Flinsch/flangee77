#include "FontLoader.h"



namespace fl7::fonts {



    /**
     * Returns a fallback name based on the font's filename.
     */
    cl7::u8string FontLoader::get_fallback_name() const
    {
        cl7::u8string_view fallback = _file_path;

        size_t pos = fallback.find_last_of(u8"/\\");
        if (pos != cl7::u8string_view::npos)
            fallback = fallback.substr(pos + 1);

        pos = fallback.find_last_of(u8'.');
        if (pos != cl7::u8string_view::npos)
            fallback = fallback.substr(0, pos);

        return cl7::u8string{fallback};
    }



    cl7::io::IReadable& FontLoader::_open(size_t byte_offset)
    {
        if (!_file.is_readable())
            _file.open(_file_path, cl7::io::OpenMode::Read);

        _file.set_read_position(byte_offset);

        return _file;
    }

    void FontLoader::_close()
    {
        _file.close();
    }



} // namespace fl7::fonts
