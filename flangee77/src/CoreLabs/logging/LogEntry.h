#pragma once
#ifndef CL7_LOGGING_LOGENTRY_H
#define CL7_LOGGING_LOGENTRY_H

#include "./LogType.h"

#include <CoreLabs/string.h>

#include <source_location>



namespace cl7 {
namespace logging {



/**
 * Attention! If you want to store the log message for later access, you should
 * convert the string_view to a string managed by you. This should actually go
 * without saying, but in this context "entry" (of "LogEntry") could be a bit
 * misleading, because it sounds so much like "storing", so I'll mention it
 * explicitly here.
 */
struct LogEntry
{

    /** The log message itself (as a string_view not suitable for storing for later use). */
    cl7::string_view message;
    /** The log type (error, warning, etc.). */
    LogType type;

    /** The (relative) path to the file the log was generated in. */
    const cl7::achar_type* file_path = nullptr;
    /** The line number the log was generated at. */
    unsigned line_number = 0;
    /** The function name the log was generated from. */
    const cl7::achar_type* function_name = nullptr;

}; // class LogEntry



} // namespace logging
} // namespace cl7

#endif // CL7_LOGGING_LOGENTRY_H
