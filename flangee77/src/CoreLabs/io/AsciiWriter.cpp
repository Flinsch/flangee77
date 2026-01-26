#include "AsciiWriter.h"



namespace cl7::io {



    /**
     * Prepares an ASCII writer for writing to the specified writable object.
     */
    AsciiWriter::AsciiWriter(IWritable* writable) noexcept
        : _writable(writable)
    {
    }

    /**
     * Attempts to write the given ASCII string and returns the number of ASCII
     * characters/bytes actually transferred.
     */
    size_t AsciiWriter::write(cl7::astring_view text)
    {
        return _writable->write(cl7::make_byte_view(text));
    }

    /**
     * Attempts to write the given ASCII string followed by a line break (`\n`, or
     * whatever you specify). The number of ASCII characters/bytes actually
     * transferred is then returned, without the appended line break.
     */
    size_t AsciiWriter::write_line(cl7::astring_view line, cl7::astring_view line_break)
    {
        const auto written = write(line);
        if (!line_break.empty())
            _writable->write(cl7::make_byte_view(line_break));
        return written;
    }

    /**
     * Attempts to write a single character. Returns 1 on success, 0 on failure.
     */
    size_t AsciiWriter::write_char(cl7::achar_t chr)
    {
        return _writable->write(static_cast<std::byte>(chr));
    }



} // namespace cl7::io
