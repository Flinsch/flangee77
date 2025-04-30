#include "Diagnostics.h"



namespace dl7::syntax {



    /**
     * Tries to transform the given byte offset into a textual source context.
     * In the basic version, an "empty" source context is returned. The logic can be
     * overridden in derived classes to refine/customize the behavior.
     */
    SourceContext Diagnostics::make_source_context(size_t source_offset) const
    {
        return {};
    }



    /**
     * Clears all diagnostics and the number of errors and warnings.
     */
    void Diagnostics::clear()
    {
        *this = Diagnostics(_log_context);
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
     * Adds the specified diagnostic with a simple message.
     */
    void Diagnostics::add(Diagnostic::Severity severity, cl7::u8string_view message)
    {
        add({.severity = severity, .message = cl7::u8string(message)});
    }

    /**
     * Adds the specified diagnostic with a message and a byte offset into the
     * source text. Depending on the overridable function `make_source_context`, a
     * basic source location or a concrete textual source context may be pulled in.
     */
    void Diagnostics::add(Diagnostic::Severity severity, cl7::u8string_view message, size_t source_offset)
    {
        add({.severity = severity, .message = cl7::u8string(message), .source_context = make_source_context(source_offset)});
    }

    /**
     * Adds the specified diagnostic with a message and a basic source location.
     */
    void Diagnostics::add(Diagnostic::Severity severity, cl7::u8string_view message, const SourceLocation& source_location)
    {
        add({.severity = severity, .message = cl7::u8string(message), .source_context = {.location = source_location, .line_extract = {}}});
    }

    /**
     * Adds the specified diagnostic with a message and an explicit textual source
     * context.
     */
    void Diagnostics::add(Diagnostic::Severity severity, cl7::u8string_view message, const SourceContext& source_context)
    {
        add({.severity = severity, .message = cl7::u8string(message), .source_context = source_context});
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
