#include "Diagnostics.h"



namespace dl7::syntax {



    /**
     * Clears all diagnostics and the number of errors and warnings.
     */
    void Diagnostics::clear()
    {
        *this = Diagnostics();
    }

    /**
     * Adds the specified diagnostic.
     */
    void Diagnostics::add(const Diagnostic& diagnostic)
    {
        _diagnostics.push_back(diagnostic);

        switch (diagnostic.severity)
        {
        case Diagnostic::Severity::Error:
            ++_error_count;
            break;
        case Diagnostic::Severity::Warning:
            ++_warning_count;
            break;
        default:
            break; // Nothing to do here.
        }

        diagnostic.try_log(_log_context);
    }

    /**
     * Adds the specified diagnostic.
     */
    void Diagnostics::add(Diagnostic::Severity severity, cl7::u8string_view message, const SourceLocation& source_location)
    {
        add({.severity=severity, .message=cl7::u8string(message), .source_location=source_location});
    }

    /**
     * Tries to log all diagnostic entries according to the set log context.
     */
    void Diagnostics::try_log_all() const
    {
        try_log_all(_log_context);
    }

    /**
     * Tries to log all diagnostic entries according to the specified log context.
     */
    void Diagnostics::try_log_all(cl7::logging::LogContext log_context) const
    {
        for (const auto& diagnostic : _diagnostics)
            diagnostic.try_log(log_context);
    }



} // namespace dl7::syntax
