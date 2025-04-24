#ifndef DL7_SYNTAX_DIAGNOSTICS_H
#define DL7_SYNTAX_DIAGNOSTICS_H

#include "./Diagnostic.h"

#include <CoreLabs/logging/LogContext.h>

#include <vector>



namespace dl7::syntax {



/**
 * Manages a collection of diagnostic entries. This class maintains a log context
 * for newly added diagnostics and keeps a record of diagnostic entries along with
 * respective counts of errors and warnings.
 */
class Diagnostics
{

public:
    Diagnostics() = default;
    explicit Diagnostics(cl7::logging::LogContext log_context) : _log_context(log_context) {}



    /**
     * Clears all diagnostics and the number of errors and warnings.
     */
    void clear();

    /**
     * Adds the specified diagnostic.
     */
    void add(const Diagnostic& diagnostic);

    /**
     * Adds the specified diagnostic.
     */
    void add(Diagnostic::Severity severity, cl7::u8string_view message, const SourceLocation& source_location);

    /**
     * Returns the "list" of all diagnostics.
     */
    const std::vector<Diagnostic>& get_all() const { return _diagnostics; }

    /**
     * Returns the total number of diagnostics.
     */
    size_t get_count() const { return _diagnostics.size(); }

    /**
     * Returns the number of errors.
     */
    size_t get_error_count() const { return _error_count; }

    /**
     * Returns the number of warnings.
     */
    size_t get_warning_count() const { return _warning_count; }

    /**
     * Tries to log all diagnostic entries.
     */
    void try_log_all() const;

    /**
     * Tries to log all diagnostic entries according to the specified log context.
     */
    void try_log_all(cl7::logging::LogContext log_context) const;

    /**
     * Returns the log context (to be used for newly added diagnostic entries).
     */
    cl7::logging::LogContext get_log_context() const { return _log_context; }



private:
    /** The log context to be used for newly added diagnostic entries. */
    cl7::logging::LogContext _log_context;

    /** The "list" of diagnostics. */
    std::vector<Diagnostic> _diagnostics;

    /** The number of errors. */
    size_t _error_count = 0;

    /** The number of warnings. */
    size_t _warning_count = 0;

}; // class Diagnostics



} // namespace dl7::syntax

#endif // DL7_SYNTAX_DIAGNOSTICS_H
