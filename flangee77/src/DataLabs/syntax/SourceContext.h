#ifndef DL7_SYNTAX_SOURCECONTEXT_H
#define DL7_SYNTAX_SOURCECONTEXT_H

#include "./SourceLocation.h"



namespace dl7::syntax {



/**
 * Represents a textual context in a source text, as location (line and column
 * numbers) along with the corresponding line of code.
 */
struct SourceContext
{

    /** The location in the source text (line and column). */
    SourceLocation location;

    /** The line of text extracted from the source at the given location. */
    cl7::u8string line_extract;



    /**
     * Constructs a source context from a byte offset into the given source text.
     */
    static SourceContext from_source_offset(cl7::u8string_view source, size_t offset);

}; // struct SourceContext



} // namespace dl7::syntax

#endif // DL7_SYNTAX_SOURCECONTEXT_H
