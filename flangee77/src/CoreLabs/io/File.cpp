#include "File.h"

#include <algorithm>
#include <filesystem>



namespace cl7::io {



    static_assert(static_cast<unsigned>(SeekMode::Begin) == static_cast<unsigned>(std::ios::beg));
    static_assert(static_cast<unsigned>(SeekMode::Current) == static_cast<unsigned>(std::ios::cur));
    static_assert(static_cast<unsigned>(SeekMode::End) == static_cast<unsigned>(std::ios::end));



    File::File(cl7::u8string_view path, OpenMode open_mode)
        : File()
    {
        open(path, open_mode);
    }


    /**
     * Opens the specified file for reading and writing.
     */
    bool File::open(cl7::u8string_view path, OpenMode open_mode)
    {
        close();

        std::ios::openmode om = std::ios::binary;
        if ((open_mode & OpenMode::Read) == OpenMode::Read)
            om |= std::ios::in;
        if ((open_mode & OpenMode::Write) == OpenMode::Write)
            om |= std::ios::out;
        if ((open_mode & OpenMode::Append) == OpenMode::Append)
            om |= std::ios::ate;
        if ((open_mode & OpenMode::Truncate) == OpenMode::Truncate)
            om |= std::ios::trunc;

        _fstream.open(std::filesystem::path(path), om);
        if (!_fstream.is_open())
        {
            close();
            return false;
        }

        _path = cl7::u8string(path);
        _open_mode = open_mode;

        if (is_writable() && (open_mode & OpenMode::Truncate) == OpenMode::Truncate)
            _size = 0;
        else
            _size = std::filesystem::file_size(_path);

        if (is_writable() && (open_mode & OpenMode::Append) == OpenMode::Append)
            _write_position = _size;
        else
            _write_position = 0;

        _read_position = 0;

        return true;
    }

    /**
     * Closes the file (if opened before).
     */
    void File::close()
    {
        if (_fstream.is_open())
            _fstream.close();

        _size = 0;
        _read_position = 0;
        _write_position = 0;
    }


    /**
     * Checks whether the file is readable, i.e. whether it has been (successfully)
     * opened for reading and no previous read operation caused an error, e.g., due
     * to I/O failures at the OS level or loss of stream buffer integrity.
     */
    bool File::is_readable() const
    {
        return (_open_mode & OpenMode::Read) == OpenMode::Read && _fstream.is_open() && !_fstream.bad();
    }

    /**
     * Checks whether EOF has been reached, so that there is no more data to read at
     * the current byte position.
     */
    bool File::is_eof() const
    {
        return get_readable_bytes_remaining() == 0;
    }

    /**
     * Returns the total readable size of the file, in bytes.
     */
    size_t File::get_total_readable_bytes() const
    {
        if (!is_readable())
            return 0;

        return _size;
    }

    /**
     * Returns the number of bytes that can be read from the file at the current
     * read position.
     */
    size_t File::get_readable_bytes_remaining() const
    {
        const auto total_readable_bytes = get_total_readable_bytes();
        assert(_read_position <= total_readable_bytes);
        if (_read_position >= total_readable_bytes)
            return 0;
        return total_readable_bytes - _read_position;
    }

    /**
     * Returns the current read position within the file (in bytes, relative to the
     * beginning).
     */
    size_t File::get_read_position() const
    {
        return _read_position;
    }

    /**
     * (Re)sets the current read position within the file (in bytes, relative to the
     * beginning). Returns the new/current byte position.
     */
    size_t File::set_read_position(size_t position)
    {
        if (!is_readable())
            return 0;

        _fstream.seekg(static_cast<ptrdiff_t>(position), static_cast<std::ios::seekdir>(SeekMode::Begin));
        _read_position = _fstream.tellg();
        return _read_position;
    }

    /**
     * (Re)sets the current read position within the file by moving it by the
     * specified bytes relative to the beginning, end, or current position, as
     * specified. Returns the new/current byte position.
     */
    size_t File::seek_read(ptrdiff_t relative, SeekMode seek_mode)
    {
        if (!is_readable())
            return 0;

        _fstream.seekg(relative, static_cast<std::ios::seekdir>(seek_mode));
        _read_position = _fstream.tellg();
        return _read_position;
    }

    /**
     * Reads data from the file (at the current position) into the specified buffer
     * and returns the number of bytes actually read.
     */
    size_t File::read(cl7::byte_span buffer)
    {
        if (!is_readable())
            return 0;

        auto position = _read_position;
        _fstream.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
        _read_position = _fstream.tellg();
        return _read_position - position;
    }

    /**
     * Reads a single byte from the file (at the current position) and returns the
     * number of bytes actually read (i.e. 0 or 1).
     */
    size_t File::read(std::byte& byte)
    {
        return read(cl7::byte_span(&byte, 1));
    }

    /**
     * "Peeks" a single byte from the file (at the current position) without
     * extracting it. Returns the number of bytes that would have been extracted if
     * actually read (i.e. 0 or 1).
     */
    size_t File::peek(std::byte& byte)
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
     * Checks whether the file is writable, i.e. whether it has been (successfully)
     * opened for writing and no previous read operation caused an error, e.g., due
     * to I/O failures at the OS level or loss of stream buffer integrity.
     */
    bool File::is_writable() const
    {
        return (_open_mode & OpenMode::Write) == OpenMode::Write && _fstream.is_open() && !_fstream.bad();
    }

    /**
     * Returns the total writable size of the file, in bytes.
     */
    size_t File::get_total_writable_bytes() const
    {
        if (!is_writable())
            return 0;

        return static_cast<size_t>(-1);
    }

    /**
     * Returns the number of bytes that can be written to the file at the current
     * write position.
     */
    size_t File::get_writable_bytes_remaining() const
    {
        const auto total_writable_bytes = get_total_writable_bytes();
        assert(_write_position <= total_writable_bytes);
        if (_write_position >= total_writable_bytes)
            return 0;
        return total_writable_bytes - _write_position;
    }

    /**
     * Returns the current write position within the file (in bytes, relative to the
     * beginning).
     */
    size_t File::get_write_position() const
    {
        return _write_position;
    }

    /**
     * (Re)sets the current write position within the file (in bytes, relative to
     * the beginning). Returns the new/current byte position.
     */
    size_t File::set_write_position(size_t position)
    {
        if (!is_writable())
            return 0;

        _fstream.seekp(static_cast<ptrdiff_t>(position), static_cast<std::ios::seekdir>(SeekMode::Begin));
        _write_position = _fstream.tellp();
        return _write_position;
    }

    /**
     * (Re)sets the current write position within the file by moving it by the
     * specified bytes relative to the beginning, end, or current position, as
     * specified. Returns the new/current byte position.
     */
    size_t File::seek_write(ptrdiff_t relative, SeekMode seek_mode)
    {
        if (!is_writable())
            return 0;

        _fstream.seekp(relative, static_cast<std::ios::seekdir>(seek_mode));
        _write_position = _fstream.tellp();
        return _write_position;
    }

    /**
     * Writes data to the file (at the current position) from the specified buffer
     * and returns the number of bytes actually written.
     */
    size_t File::write(cl7::byte_view buffer)
    {
        if (!is_writable())
            return 0;

        auto position = _write_position;
        _fstream.write(reinterpret_cast<const char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
        _write_position = _fstream.tellp();
        return _write_position - position;
    }

    /**
     * Writes a single byte to the file (at the current position) and returns the
     * number of bytes actually written (i.e. 0 or 1).
     */
    size_t File::write(std::byte byte)
    {
        return write(cl7::byte_view(&byte, 1));
    }


    /**
     * Returns the current size of the file, in bytes.
     */
    size_t File::get_size() const
    {
        return _size;
    }

    /**
     * (Re)sets the size of the file, in bytes, and truncates or appends the
     * difference in bytes accordingly. Returns the new/current size.
     */
    size_t File::set_size(size_t size)
    {
        if (!is_writable())
            return _size;

        std::error_code ec;
        std::filesystem::resize_file(_path, size, ec);
        if (ec)
            return _size;

        _size = std::filesystem::file_size(_path);
        _read_position = std::min(_size, _read_position);
        _write_position = std::min(_size, _write_position);

        set_read_position(_read_position);
        set_write_position(_write_position);

        return _size;
    }



} // namespace cl7::io
