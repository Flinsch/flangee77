#ifndef DL7_SYNTAX_SOURCELOCATION_H
#define DL7_SYNTAX_SOURCELOCATION_H

#include <CoreLabs/string.h>



namespace dl7::syntax {



/**
 * Represents a specific location in a source text using 1-indexed line and column
 * numbers.
 */
struct SourceLocation
{

    /** The 1-indexed line number on the source text. */
    size_t line = 1;

    /** The 1-indexed column number, in terms of code points. */
    size_t column = 1;



    /**
     * Constructs a source location from a byte offset into the given source text.
     */
    static SourceLocation from_source_offset(cl7::u8string_view source, size_t offset);

}; // struct SourceLocation



} // namespace dl7::syntax

#endif // DL7_SYNTAX_SOURCELOCATION_H
