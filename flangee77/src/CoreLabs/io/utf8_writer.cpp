#include "utf8_writer.h"



namespace cl7::io {



    /**
     * Prepares a UTF-8 writer for writing to the specified file.
     */
    utf8_writer::utf8_writer(ifile* file) noexcept
        : _file(file)
    {
    }

    /**
     * Attempts to write the given UTF-8 string and returns the number of UTF-8
     * characters/bytes actually transferred.
     */
    size_t utf8_writer::write(cl7::u8string_view text)
    {
        return _file->write(cl7::byte_view(reinterpret_cast<const std::byte*>(text.data()), text.size()));
    }

    /**
     * Attempts to write the given UTF-8 string followed by a line break (`\n`). The
     * number of UTF-8 characters/bytes actually transferred is then returned,
     * without the appended line break.
     */
    size_t utf8_writer::write_line(cl7::u8string_view line)
    {
        const auto written = write(line);
        std::byte line_break[1] = {static_cast<std::byte>('\n')};
        _file->write(cl7::byte_view(line_break, 1));
        return written;
    }



} // namespace cl7::io
