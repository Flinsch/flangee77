#pragma once
#ifndef DL7_SYNTAX_SOURCELOCATION_H
#define DL7_SYNTAX_SOURCELOCATION_H

#include <CoreLabs/root.h>



namespace dl7 {
namespace syntax {



struct SourceLocation
{

    /** The 0-indexed offset position of the token's lexeme in the source text. */
    size_t offset = 0;

    /** The 1-indexed line number in the source text. */
    size_t line = 1;
    /** The 1-indexed column number in the source text. */
    size_t column = 1;

}; // struct SourceLocation



} // namespace syntax
} // namespace dl7

#endif // DL7_SYNTAX_SOURCELOCATION_H