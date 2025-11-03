#include "UnescaperErrorHandler.h"



namespace dl7::xml::detail {



    void UnescaperErrorHandler::on_invalid_utf8_encoding(cl7::u8string_view sequence) const
    {
        diagnostics->add(syntax::Diagnostic::Severity::Warning, u8"Invalid UTF-8 encoding.", token->source_offset);
    }



} // namespace dl7::xml::detail
