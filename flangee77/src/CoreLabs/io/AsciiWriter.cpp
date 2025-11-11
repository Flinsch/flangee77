#include "AsciiWriter.h"



namespace cl7::io {



    /**
     * Prepares a ASCII writer for writing to the specified writable object.
     */
    AsciiWriter::AsciiWriter(IWritable* writable) noexcept
        : _writable(writable)
    {
    }

    /**
     * Attempts to write the given ASCII string and returns the number of ASCII
     * characters/bytes actually transferred.
     */
    size_t AsciiWriter::write(cl7::astring_view text) const
    {
        return _writable->write(cl7::make_byte_view(text));
    }

    /**
     * Attempts to write the given ASCII string followed by a line break (`\n`, or
     * whatever you specify). The number of ASCII characters/bytes actually
     * transferred is then returned, without the appended line break.
     */
    size_t AsciiWriter::write_line(cl7::astring_view line, cl7::astring_view line_break) const
    {
        const auto written = write(line);
        if (!line_break.empty())
            _writable->write(cl7::make_byte_view(line_break));
        return written;
    }



} // namespace cl7::io
