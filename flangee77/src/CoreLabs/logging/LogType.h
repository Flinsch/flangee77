#ifndef CL7_LOGGING_LOGTYPE_H
#define CL7_LOGGING_LOGTYPE_H

#include <CoreLabs/root.h>



namespace cl7::logging {



enum class LogType
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
    Comment,
    Other
}; // enum class LogType



} // namespace cl7::logging

#endif // CL7_LOGGING_LOGTYPE_H
