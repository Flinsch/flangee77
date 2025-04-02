#ifndef DL7_SYNTAX_DIAGNOSTIC_H
#define DL7_SYNTAX_DIAGNOSTIC_H

#include "./SourceLocation.h"

#include <CoreLabs/logging/LogContext.h>
#include <CoreLabs/string.h>



namespace dl7::syntax {



/**
 * Represents a single diagnostic entry, such as an error, warning, or notice. This
 * structure encapsulates information about the severity, message content, and
 * source location of the diagnostic event.
 */
struct Diagnostic
{

    /**
     * Enumerates the severity levels of a diagnostic event.
     */
    enum struct Severity
    {
        /** Represents a critical issue that may, but does not necessarily, prevent further execution. */
        Error,
        /** Indicates a potential problem that may not be fatal. */
        Warning,
        /** Provides informational messages or minor issues. */
        Notice,
    };

    /** The severity level (error, warning, notice). */
    Severity severity;

    /** The error/warning/notice message. */
    cl7::u8string message;

    /** The location of the "incident" in the source text. */
    SourceLocation source_location;



    /**
     * Tries to log this diagnostic according to the specified log context.
     */
    void try_log(cl7::logging::LogContext log_context) const;

}; // struct Diagnostic



} // namespace dl7::syntax

#endif // DL7_SYNTAX_DIAGNOSTIC_H
