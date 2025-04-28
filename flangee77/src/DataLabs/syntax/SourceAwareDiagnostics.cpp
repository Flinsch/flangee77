#include "SourceAwareDiagnostics.h"



namespace dl7::syntax {



    /**
     * Tries to transform the given byte offset into a textual source context.
     * The source location is returned along with a line, which is extracted from
     * the source text based on the given information.
     */
    SourceContext SourceAwareDiagnostics::make_source_context(size_t source_offset) const
    {
        return SourceContext::from_source_offset(_source, source_offset);
    }



} // namespace dl7::syntax
