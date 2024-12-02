#include "Diagnostics.h"



namespace dl7 {
namespace syntax {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Erases all diagnostics.
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
        _diagnostics.push_back( diagnostic );

        switch ( diagnostic.severity )
        {
        case Diagnostic::Severity::Error:
            ++_error_count;
            break;
        case Diagnostic::Severity::Warning:
            ++_warning_count;
            break;
        }
    }

    /**
     * Adds the specified diagnostic.
     */
    void Diagnostics::add(Diagnostic::Severity severity, cl7::string_view message, const SourceLocation& source_location)
    {
        add( { severity, cl7::string(message), source_location } );
    }



} // namespace syntax
} // namespace dl7
