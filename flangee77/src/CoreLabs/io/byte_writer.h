#ifndef CL7_IO_BYTEWRITER_H
#define CL7_IO_BYTEWRITER_H

#include "./ifile.h"

#include "CoreLabs/byte_view.h"



namespace cl7::io {



class byte_writer
{

public:
    /**
     * Prepares a byte writer for writing to the specified file.
     */
    explicit byte_writer(ifile* file) noexcept;

    /**
     * Attempts to write the given data and returns the number of bytes actually
     * transferred.
     */
    size_t write(cl7::byte_view data) const;

private:
    /** The file to write to. */
    ifile* _file;

}; // class byte_writer



} // namespace cl7::io

#endif //CL7_IO_BYTEWRITER_H
