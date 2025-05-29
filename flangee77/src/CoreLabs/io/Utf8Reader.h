#ifndef CL7_IO_UTF8READER_H
#define CL7_IO_UTF8READER_H

#include "./IReadable.h"

#include "CoreLabs/string.h"



namespace cl7::io {



class Utf8Reader
{

public:
    /**
     * Prepares a UTF-8 reader for reading from the specified readable object.
     */
    explicit Utf8Reader(IReadable* readable) noexcept;

    /**
     * Reads all available/remaining data without checking for valid UTF-8.
     */
    cl7::u8string read() const;

    /**
     * Attempts to read the next line without checking for valid UTF-8.
     */
    cl7::u8string read_line() const;

    /**
     * Reads all available/remaining data with UTF-8 validation/"correction".
     */
    cl7::u8string read_validated() const;

    /**
     * Attempts to read the next line with UTF-8 validation/"correction".
     */
    cl7::u8string read_line_validated() const;

private:
    /** The readable source object. */
    IReadable* _readable;

}; // class Utf8Reader



} // namespace cl7::io

#endif //CL7_IO_UTF8READER_H
