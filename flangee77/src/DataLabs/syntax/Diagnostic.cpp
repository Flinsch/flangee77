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

        oss << message << " :" << source_location.line << ":" << source_location.column;

        log_context.try_log(oss.str(), log_type);
    }



} // namespace dl7::syntax
