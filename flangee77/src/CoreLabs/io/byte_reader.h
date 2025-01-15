#ifndef CL7_IO_BYTEREADER_H
#define CL7_IO_BYTEREADER_H

#include "./irom.h"

#include "CoreLabs/byte_vector.h"
#include "CoreLabs/byte_span.h"



namespace cl7::io {



class byte_reader
{

public:
    /**
     * Prepares a byte reader for reading from the specified read-only memory.
     */
    explicit byte_reader(irom* rom) noexcept;

    /**
     * Reads and returns all available/remaining data.
     */
    cl7::byte_vector read() const;

    /**
     * Attempts to fill the given data buffer and returns the number of bytes
     * actually transferred.
     */
    size_t read(cl7::byte_span data) const;

private:
    /** The read-only memory to read from. */
    irom* _rom;

}; // class byte_reader



} // namespace cl7::io

#endif //CL7_IO_BYTEREADER_H
