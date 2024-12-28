#pragma once
#ifndef DL7_SYNTAX_DIAGNOSTICS_H
#define DL7_SYNTAX_DIAGNOSTICS_H

#include "./Diagnostic.h"

#include <vector>



namespace dl7::syntax {



class Diagnostics
{

public:
    /**
     * Erases all diagnostics.
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



private:
    /** The "list" of diagnostics. */
    std::vector<Diagnostic> _diagnostics;

    /** The number of errors. */
    size_t _error_count = 0;

    /** The number of warnings. */
    size_t _warning_count = 0;

}; // class Diagnostics



} // namespace dl7::syntax

#endif // DL7_SYNTAX_DIAGNOSTICS_H
