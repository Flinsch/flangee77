#include "SourceLocation.h"

#include <CoreLabs/strings.h>



namespace dl7::syntax {



    /**
     * Constructs a source location from a byte offset into the given source text.
     */
    SourceLocation SourceLocation::from_source_offset(cl7::u8string_view source, size_t offset)
    {
        assert(offset <= source.size());
        if (offset > source.size())
            offset = source.size();

        size_t line = 1;
        size_t line_offset = 0;

        for (size_t i = 0; i < offset; )
        {
            const size_t line_break = cl7::strings::is_line_break_prefix(source.substr(i));
            if (line_break > 0)
            {
                // Line break: advance the line.
                ++line;

                // Skip second line-break character if applicable,
                // i.e. LF (`\n`) after CR (`\r`) on Windows.
                assert(line_break <= 2);
                i += line_break;

                // Reset the line offset.
                line_offset = i;
            }
            else
            {
                // "Regular" character.
                ++i;
            }
        } // for ...

        size_t column = cl7::strings::utf8_length(source.substr(line_offset, offset - line_offset)) + 1;

        return {.line = line, .column = column};
    }



} // namespace dl7::syntax
