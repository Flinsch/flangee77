#include "FontLoader.h"



namespace fl7::fonts {



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
