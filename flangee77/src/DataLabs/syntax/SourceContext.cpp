#include "SourceContext.h"

#include <CoreLabs/strings.h>



namespace dl7::syntax {



    /**
     * Constructs a source context from a byte offset into the given source text.
     */
    SourceContext SourceContext::from_source_offset(cl7::u8string_view source, size_t offset)
    {
        assert(offset <= source.size());
        if (offset > source.size())
            offset = source.size();

        size_t begin = offset;
        while (begin > 0 && !cl7::strings::is_line_break(source[begin - 1]))
            --begin;

        size_t end = offset;
        while (end < source.size() && !cl7::strings::is_line_break(source[end]))
            ++end;

        return {
            .location = SourceLocation::from_source_offset(source, offset),
            .line_extract = cl7::u8string{source.substr(begin, end - begin)},
        };
    }



} // namespace dl7::syntax
