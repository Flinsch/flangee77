#pragma once
#ifndef CL7_LOGGING_LOGENTRY_H
#define CL7_LOGGING_LOGENTRY_H

#include "./LogType.h"

#include <CoreLabs/string.h>



namespace cl7 {
namespace logging {



struct LogEntry
{

    enum class Type
    {
        Log,
        Info,
        Success,
        Warning,
        Error,
        Caption,
        Section,
        Item,
        ItemPass,
        ItemFail,
        Code,
        Meta,
        Other
    }; // enum class Type



    /** The log message itself.  */
    cl7::string_view message;
    /** The log type (error, warning, etc.). */
    LogType type;

    /** The (relative) path to the file the log was generated in. */
    const cl7::char_type* file_path = nullptr;
    /** The line number the log was generated at. */
    unsigned line_number = 0;
    /** The function name the log was generated from. */
    const cl7::char_type* function_name = nullptr;



}; // class LogEntry



} // namespace logging
} // namespace cl7

#endif // CL7_LOGGING_LOGENTRY_H
