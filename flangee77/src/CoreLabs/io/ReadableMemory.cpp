#include "ReadableMemory.h"



namespace cl7::io {



    ReadableMemory::ReadableMemory(cl7::byte_vector&& source_data)
        : _data(std::move(source_data))
    {
    }

    ReadableMemory::ReadableMemory(cl7::byte_view source_data)
        : _data(source_data.begin(), source_data.end())
    {
    }

    void ReadableMemory::swap(ReadableMemory& other) noexcept
    {
        _data.swap(other._data);
        std::swap(_read_position, other._read_position);
    }


    /**
     * Checks whether EOF has been reached, so that there is no more data to read at
     * the current byte position.
     */
    bool ReadableMemory::is_eof() const
    {
        return _read_position >= _data.size();
    }

    /**
     * Returns the total readable size of the source data, in bytes.
     */
    size_t ReadableMemory::get_total_readable_bytes() const
    {
        return _data.size();
    }

    /**
     * Returns the number of bytes that can be read from the source data at the
     * current read position.
     */
    size_t ReadableMemory::get_readable_bytes_remaining() const
    {
        if (_read_position >= _data.size())
            return 0;
        return _data.size() - _read_position;
    }

    /**
     * Returns the current read position within the source data (in bytes, relative
     * to the beginning).
     */
    size_t ReadableMemory::get_read_position() const
    {
        return _read_position;
    }

    /**
     * (Re)sets the current read position within the source data (in bytes, relative
     * to the beginning). Returns the new/current byte position.
     */
    size_t ReadableMemory::set_read_position(size_t position)
    {
        _read_position = position;
        return _read_position;
    }

    /**
     * (Re)sets the current read position within the source data by moving it by the
     * specified bytes relative to the beginning, end, or current position, as
     * specified. Returns the new/current byte position.
     */
    size_t ReadableMemory::seek_read(ptrdiff_t relative, SeekMode seek_mode)
    {
        size_t base_position = 0;

        switch (seek_mode)
        {
        case SeekMode::Current:
            base_position = _read_position;
            break;
        case SeekMode::End:
            base_position = _data.size();
            break;
        default:
            assert(seek_mode == SeekMode::Begin);
            // Nothing to do here.
            // base_position is already 0.
            break;
        }

        if (relative < 0 && base_position + relative > base_position) // Underflow?
            _read_position = 0;
        else
            _read_position = base_position + relative;
        return _read_position;
    }

    /**
     * Reads data from the source data (at the current position) into the specified
     * buffer and returns the number of bytes actually read.
     */
    size_t ReadableMemory::read(cl7::byte_span buffer)
    {
        size_t bytes = (std::min)(get_readable_bytes_remaining(), buffer.size());
        std::memcpy(buffer.data(), _data.data() + _read_position, bytes);
        _read_position += bytes;
        return bytes;
    }

    /**
     * Reads a single byte from the source data (at the current position) and
     * returns the number of bytes actually read (i.e. 0 or 1).
     */
    size_t ReadableMemory::read(std::byte& byte)
    {
        return read(cl7::byte_span(&byte, 1));
    }

    /**
     * "Peeks" a single byte from the source data (at the current position) without
     * extracting it. Returns the number of bytes that would have been extracted if
     * actually read (i.e. 0 or 1).
     */
    size_t ReadableMemory::peek(std::byte& byte)
    {
        if (get_readable_bytes_remaining() == 0)
            return 0;

        byte = _data[_read_position];
        return 1;
    }



} // namespace cl7::io
