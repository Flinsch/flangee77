#ifndef CL7_IO_UTF8WRITER_H
#define CL7_IO_UTF8WRITER_H

#include "./IWritable.h"

#include "CoreLabs/string.h"



namespace cl7::io {



class Utf8Writer
{

public:
    /**
     * Prepares a UTF-8 writer for writing to the specified writable object.
     */
    explicit Utf8Writer(IWritable* writable) noexcept;

    /**
     * Attempts to write the given UTF-8 string and returns the number of UTF-8
     * characters/bytes actually transferred.
     */
    size_t write(cl7::u8string_view text) const;

    /**
     * Attempts to write the given UTF-8 string followed by a line break (`\n`, or
     * whatever you specify). The number of UTF-8 characters/bytes actually
     * transferred is then returned, without the appended line break.
     */
    size_t write_line(cl7::u8string_view line, cl7::u8string_view line_break = u8"\n") const;

private:
    /** The writable target object. */
    IWritable* _writable;

}; // class Utf8Writer



} // namespace cl7::io

#endif // CL7_IO_UTF8WRITER_H
