#include "file.h"

#include <algorithm>
#include <filesystem>



namespace cl7::io {



    file::file(const cl7::u8string& path, open_mode open_mode)
        : file()
    {
        open(path, open_mode);
    }


    /**
     * Checks whether the file is "open" and can be used.
     */
    bool file::is_good() const
    {
        return _fstream.is_open();
    }

    /**
     * Checks whether the current byte position exceeds the size of the file.
     */
    bool file::is_eof() const
    {
        return _position >= _size;
    }

    /**
     * (Re)sets the current byte position within the file (relative to the beginning).
     * Returns the new/current byte position.
     */
    size_t file::set_position(size_t position)
    {
        if (!is_good())
            return 0;

        _seek_position(position);
        _position = _tell_position();
        return _position;
    }

    /**
     * (Re)sets the current byte position within the file by moving it by the
     * specified bytes relative to the file's beginning, end, or current position,
     * as specified.
     * Returns the new/current byte position.
     */
    size_t file::seek(ptrdiff_t relative, const seek_mode seek_mode)
    {
        if (!is_good())
            return 0;

        _seek_position(relative, seek_mode);
        _position = _tell_position();
        return _position;
    }

    /**
     * Reads data from the file (at the current position) into the specified buffer
     * and returns the number of bytes transferred.
     */
    size_t file::read(cl7::byte_span buffer)
    {
        if (!is_readable())
            return 0;

        auto position = _position;
        _fstream.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));

        _position = _fstream.tellg();
        _fstream.seekp(static_cast<std::streamoff>(_position));

        return _position - position;
    }

    /**
     * Reads a single byte from the file (at the current position) and returns the
     * number of bytes transferred (i.e. 0 or 1).
     */
    size_t file::read(std::byte& byte)
    {
        return read(cl7::byte_span(&byte, 1));
    }

    /**
     * "Peeks" a single byte from the file (at the current position) without
     * extracting it. Returns the number of bytes that would have been extracted if
     * actually read (i.e. 0 or 1).
     */
    size_t file::peek(std::byte& byte)
    {
        if (!is_readable())
            return 0;

        auto result = _fstream.peek();
        if (result < 0)
            return 0;

        assert(result <= 0xff);
        byte = static_cast<std::byte>(result);
        return 1;
    }

    /**
     * Writes data to the file (at the current position) from the specified buffer
     * and returns the number of bytes transferred.
     */
    size_t file::write(cl7::byte_view buffer)
    {
        if (!is_writable())
            return 0;

        auto position = _position;
        _fstream.write(reinterpret_cast<const char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));

        _position = _fstream.tellp();
        _fstream.seekg(static_cast<std::streamoff>(_position));

        return _position = position;
    }

    /**
     * Writes a single byte to the file (at the current position) and returns the
     * number of bytes transferred (i.e. 0 or 1).
     */
    size_t file::write(std::byte byte)
    {
        return write(cl7::byte_view(&byte, 1));
    }

    /**
     * (Re)sets the size of the file, in bytes, and truncates or appends the
     * difference in bytes accordingly.
     * Returns the new/current file size.
     */
    size_t file::set_size(size_t size)
    {
        if (!is_good())
            return _size;

        std::error_code ec;
        std::filesystem::resize_file(_path, size, ec);
        if (ec)
            return _size;

        _size = std::filesystem::file_size(_path);
        _position = std::min(_size, _position);

        _seek_position(_position);
        _position = _tell_position();

        return _size;
    }

    /**
     * Checks whether the file is "open" and can be read.
     */
    bool file::is_readable() const
    {
        return is_good() && (_open_mode & open_mode::read) == open_mode::read;
    }

    /**
     * Checks whether the file is "open" and can be written.
     */
    bool file::is_writable() const
    {
        return is_good() && (_open_mode & open_mode::write) == open_mode::write;
    }


    /**
     * Opens the specified file for reading and writing.
     */
    bool file::open(const cl7::u8string& path, open_mode open_mode)
    {
        close();

        std::ios::openmode om = std::ios::binary;
        if ((open_mode & open_mode::read) == open_mode::read)
            om |= std::ios::in;
        if ((open_mode & open_mode::write) == open_mode::write)
            om |= std::ios::out;
        if ((open_mode & open_mode::append) == open_mode::append)
            om |= std::ios::ate;
        if ((open_mode & open_mode::truncate) == open_mode::truncate)
            om |= std::ios::trunc;

        _fstream.open(std::filesystem::path(path), om);
        if (!is_good())
        {
            close();
            return false;
        }

        _path = cl7::u8string(path);
        _open_mode = open_mode;

        _position = _tell_position();
        _seek_position(0, seek_mode::end);
        _size = _tell_position();
        _seek_position(_position);

        return true;
    }

    /**
     * Closes the file (if opened before).
     */
    void file::close()
    {
        if (_fstream.is_open())
            _fstream.close();

        _size = 0;
        _position = 0;
    }



    size_t file::_tell_position()
    {
        if (is_readable()) return _fstream.tellg();
        if (is_writable()) return _fstream.tellp();
        return 0;
    }

    void file::_seek_position(size_t position)
    {
        _seek_position(static_cast<ptrdiff_t>(position), seek_mode::begin);
    }

    void file::_seek_position(ptrdiff_t relative, const seek_mode seek_mode)
    {
        static_assert(static_cast<unsigned>(seek_mode::begin) == static_cast<unsigned>(std::ios::beg));
        static_assert(static_cast<unsigned>(seek_mode::current) == static_cast<unsigned>(std::ios::cur));
        static_assert(static_cast<unsigned>(seek_mode::end) == static_cast<unsigned>(std::ios::end));

        if (is_readable()) _fstream.seekg(relative, static_cast<std::ios::seekdir>(seek_mode));
        if (is_writable()) _fstream.seekp(relative, static_cast<std::ios::seekdir>(seek_mode));
    }



} // namespace cl7::io
