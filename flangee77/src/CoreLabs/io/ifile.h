#ifndef CL7_IO_IFILE_H
#define CL7_IO_IFILE_H
#include "./irom.h"

#include <CoreLabs/byte_view.h>



namespace cl7::io {



class ifile
    : public irom
{

public:
    ~ifile() override = default;


    /**
     * Writes data to the file (at the current position) from the specified buffer
     * and returns the number of bytes transferred.
     */
    virtual size_t write(cl7::byte_view buffer) = 0;

    /**
     * Writes a single byte to the file (at the current position) and returns the
     * number of bytes transferred (i.e. 0 or 1).
     */
    virtual size_t write(std::byte byte) = 0;

    /**
     * (Re)sets the size of the file, in bytes, and truncates or appends the
     * difference in bytes accordingly.
     * Returns the new/current file size.
     */
    virtual size_t set_size(size_t size) = 0;

    /**
     * Checks whether the file is "open" and can be read.
     */
    virtual bool is_readable() const = 0;

    /**
     * Checks whether the file is "open" and can be written.
     */
    virtual bool is_writable() const = 0;

}; // class ifile



} // namespace cl7::io

#endif // CL7_IO_IFILE_H
