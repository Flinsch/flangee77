#include "byte_writer.h"



namespace cl7::io {



    /**
     * Prepares a byte writer for writing to the specified file.
     */
    byte_writer::byte_writer(ifile* file) noexcept
        : _file(file)
    {
    }

    /**
     * Attempts to write a single byte. Returns 1 on success, 0 on failure.
     */
    size_t byte_writer::write_byte(std::byte byte) const
    {
        return _file->write(cl7::make_byte_view(&byte));
    }

    /**
     * Attempts to write multiple bytes from a buffer. Returns the number of bytes
     * actually written.
     */
    size_t byte_writer::write_bytes(cl7::byte_view data) const
    {
        return _file->write(data);
    }



} // namespace cl7::io
