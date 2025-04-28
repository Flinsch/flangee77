#ifndef DL7_SYNTAX_SOURCEAWAREDIAGNOSTICS_H
#define DL7_SYNTAX_SOURCEAWAREDIAGNOSTICS_H
#include "./Diagnostics.h"

#include <CoreLabs/string.h>



namespace dl7::syntax {



/**
 * In addition to the basic functionalities, this class also holds a reference to
 * the source text to be processed and offers an additional function for adding
 * diagnostic entries related to this source text.
 */
class SourceAwareDiagnostics
    : public Diagnostics
{

public:
    explicit SourceAwareDiagnostics(cl7::u8string_view source, cl7::logging::LogContext log_context = {}) : Diagnostics(log_context), _source(source) {}
    ~SourceAwareDiagnostics() override = default;



    /**
     * Tries to transform the given byte offset into a textual source context.
     * The source location is returned along with a line, which is extracted from
     * the source text based on the given information.
     */
    SourceContext make_source_context(size_t source_offset) const override;



    /**
     * Returns a "reference" to the source text to be processed.
     */
    cl7::u8string_view get_source() const { return _source; }



private:
    /** A "reference" to the source text to be processed. */
    cl7::u8string_view _source;

}; // class SourceAwareDiagnostics



} // namespace dl7::syntax

#endif // DL7_SYNTAX_SOURCEAWAREDIAGNOSTICS_H
