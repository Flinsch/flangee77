#ifndef CL7_IO_ASCIIWRITER_H
#define CL7_IO_ASCIIWRITER_H

#include "./IWritable.h"

#include "CoreLabs/string.h"



namespace cl7::io {



class AsciiWriter
{

public:
    /**
     * Prepares an ASCII writer for writing to the specified writable object.
     */
    explicit AsciiWriter(IWritable* writable) noexcept;

    /**
     * Attempts to write the given ASCII string and returns the number of ASCII
     * characters/bytes actually transferred.
     */
    size_t write(cl7::astring_view text);

    /**
     * Attempts to write the given ASCII string followed by a line break (`\n`, or
     * whatever you specify). The number of ASCII characters/bytes actually
     * transferred is then returned, without the appended line break.
     */
    size_t write_line(cl7::astring_view line, cl7::astring_view line_break = "\n");

    /**
     * Attempts to write a single character. Returns 1 on success, 0 on failure.
     */
    size_t write_char(cl7::achar_t chr);

private:
    /** The writable target object. */
    IWritable* _writable;

}; // class AsciiWriter



} // namespace cl7::io

#endif // CL7_IO_ASCIIWRITER_H
