#include "Utf8Writer.h"



namespace cl7::io {



    /**
     * Prepares a UTF-8 writer for writing to the specified writable object.
     */
    Utf8Writer::Utf8Writer(IWritable* writable) noexcept
        : _writable(writable)
    {
    }

    /**
     * Attempts to write the given UTF-8 string and returns the number of UTF-8
     * characters/bytes actually transferred.
     */
    size_t Utf8Writer::write(cl7::u8string_view text) const
    {
        return _writable->write(cl7::make_byte_view(text));
    }

    /**
     * Attempts to write the given UTF-8 string followed by a line break (`\n`, or
     * whatever you specify). The number of UTF-8 characters/bytes actually
     * transferred is then returned, without the appended line break.
     */
    size_t Utf8Writer::write_line(cl7::u8string_view line, cl7::u8string_view line_break) const
    {
        const auto written = write(line);
        if (!line_break.empty())
            _writable->write(cl7::make_byte_view(line_break));
        return written;
    }

    /**
     * Attempts to write a single character (code unit). Returns 1 on success, 0 on failure.
     */
    size_t Utf8Writer::write_char(cl7::u8char_t chr) const
    {
        return _writable->write(static_cast<std::byte>(chr));
    }



} // namespace cl7::io
