#pragma once
#ifndef DL7_SYNTAX_DIAGNOSTIC_H
#define DL7_SYNTAX_DIAGNOSTIC_H

#include "./SourceLocation.h"

#include <CoreLabs/string.h>



namespace dl7 {
namespace syntax {



struct Diagnostic
{

    enum class Severity
    {
        Error,
        Warning,
        Notice,
    };

    /** The severity level (error, warning, notice). */
    Severity severity;

    /** The error/warning/notice message. */
    cl7::string message;

    /** The location of the "incident" in the source text. */
    SourceLocation source_location;

}; // struct Diagnostic



} // namespace syntax
} // namespace dl7

#endif // DL7_SYNTAX_DIAGNOSTIC_H
