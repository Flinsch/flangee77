#ifndef CL7_IO_UTF8READER_H
#define CL7_IO_UTF8READER_H

#include "./irom.h"

#include "CoreLabs/string.h"



namespace cl7::io {



class utf8_reader
{

public:
    /**
     * Prepares a UTF-8 reader for reading from the specified read-only memory.
     */
    explicit utf8_reader(irom* rom) noexcept;

    /**
     * Reads and returns all available/remaining data.
     */
    cl7::u8string read() const;

    /**
     * Attempts to read the next line.
     */
    cl7::u8string read_line() const;

private:
    /** The read-only memory to read from. */
    irom* _rom;

}; // class utf8_reader



} // namespace cl7::io

#endif //CL7_IO_UTF8READER_H
