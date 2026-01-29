#include "WritableMemory.h"



namespace cl7::io {



    void WritableMemory::swap(WritableMemory& other) noexcept
    {
        _data.swap(other._data);
        std::swap(_write_position, other._write_position);
    }

    /**
     * Special swap operation. The data is essentially "exported" and replaced with
     * any other data. The write position is adjusted if necessary.
     */
    void WritableMemory::swap_data(cl7::byte_vector& data) noexcept
    {
        _data.swap(data);
        _write_position = std::min(_write_position, _data.size());
    }


    /**
     * Returns the total writable size of the target, in bytes.
     */
    size_t WritableMemory::get_total_writable_bytes() const
    {
        return static_cast<size_t>(-1);
    }

    /**
     * Returns the number of bytes that can be written to the target at the current
     * write position.
     */
    size_t WritableMemory::get_writable_bytes_remaining() const
    {
        const auto total_writable_bytes = get_total_writable_bytes();
        assert(_write_position <= total_writable_bytes);
        if (_write_position >= total_writable_bytes)
            return 0;
        return total_writable_bytes - _write_position;
    }

    /**
     * Returns the current write position within the target (in bytes, relative to
     * the beginning).
     */
    size_t WritableMemory::get_write_position() const
    {
        return _write_position;
    }

    /**
     * (Re)sets the current write position within the target (in bytes, relative to
     * the beginning). Returns the new/current byte position.
     */
    size_t WritableMemory::set_write_position(size_t position)
    {
        _write_position = position;
        return _write_position;
    }

    /**
     * (Re)sets the current write position within the target by moving it by the
     * specified bytes relative to the beginning, end, or current position, as
     * specified. Returns the new/current byte position.
     */
    size_t WritableMemory::seek_write(ptrdiff_t relative, SeekMode seek_mode)
    {
        size_t base_position = 0;

        switch (seek_mode)
        {
        case SeekMode::Current:
            base_position = _write_position;
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
            _write_position = 0;
        else
            _write_position = base_position + relative;
        return _write_position;
    }

    /**
     * Writes data to the target (at the current position) from the specified buffer
     * and returns the number of bytes actually written.
     */
    size_t WritableMemory::write(cl7::byte_view buffer)
    {
        size_t bytes = (std::min)(get_writable_bytes_remaining(), buffer.size());
        if (_write_position + bytes > _data.size())
            _data.resize(_write_position + bytes);
        std::memcpy(_data.data() + _write_position, buffer.data(), bytes);
        _write_position += bytes;
        return bytes;
    }

    /**
     * Writes a single byte to the target (at the current position) and returns the
     * number of bytes actually written (i.e. 0 or 1).
     */
    size_t WritableMemory::write(std::byte byte)
    {
        return write(cl7::byte_view(&byte, 1));
    }



} // namespace cl7::io
