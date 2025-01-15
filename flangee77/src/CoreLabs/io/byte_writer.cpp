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
     * Attempts to write the given data and returns the number of bytes actually
     * transferred.
     */
    size_t byte_writer::write(cl7::byte_view data)
    {
        return _file->write(data);
    }



} // namespace cl7::io
