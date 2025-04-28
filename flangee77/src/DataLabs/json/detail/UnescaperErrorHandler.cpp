#include "UnescaperErrorHandler.h"



namespace dl7::json::detail {



    void UnescaperErrorHandler::on_invalid_utf8_encoding(cl7::u8string_view sequence) const
    {
        diagnostics->add(syntax::Diagnostic::Severity::Warning, u8"Invalid UTF-8 encoding.", token.source_offset);
    }

    void UnescaperErrorHandler::on_invalid_utf16_encoding(cl7::u16string_view sequence) const
    {
        diagnostics->add(syntax::Diagnostic::Severity::Warning, u8"Invalid UTF-16 encoding.", token.source_offset);
    }



} // namespace dl7::json::detail
