#include "Diagnostic.h"

#include <MathLabs/functions.h>

#include <CoreLabs/sstream.h>



namespace dl7::syntax {



    /**
     * Tries to log this diagnostic according to the specified log context.
     */
    void Diagnostic::try_log(cl7::logging::LogContext log_context) const
    {
        cl7::u8osstream oss;
        cl7::logging::LogType log_type;

        if (!source_context.line_extract.empty())
        {
            oss << source_context.location.line << ":" << source_context.location.column << ": ";
        }

        switch (severity)
        {
        case Severity::Error:
            oss << "[ERROR] ";
            log_type = cl7::logging::LogType::Error;
            break;
        case Severity::Warning:
            oss << "[WARNING] ";
            log_type = cl7::logging::LogType::Warning;
            break;
        case Severity::Notice:
            oss << "[NOTICE] ";
            log_type = cl7::logging::LogType::Info;
            break;
        default:
            assert(false);
        }

        oss << message;

        log_context.try_log(oss.str(), log_type);


        if (source_context.line_extract.empty())
            return;


        oss.str(u8"");

        oss << " " << source_context.location.line << " | " << source_context.line_extract;

        log_context.try_log(oss.str(), log_type);


        oss.str(u8"");

        oss << " ";
        for (size_t i = 0; i < ml7::decimal_digits(source_context.location.line); ++i)
            oss << " ";
        oss << " | ";
        for (size_t i = 0; i < source_context.location.column - 1; ++i)
            oss << " ";
        oss << "^";

        log_context.try_log(oss.str(), log_type);
    }



} // namespace dl7::syntax
