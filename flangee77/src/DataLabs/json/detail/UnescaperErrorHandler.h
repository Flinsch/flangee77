#ifndef DL7_JSON_DETAIL_UNESCAPERERRORHANDLER_H
#define DL7_JSON_DETAIL_UNESCAPERERRORHANDLER_H

#include "../util/Unescaper.h"

#include <DataLabs/syntax/Diagnostics.h>
#include <DataLabs/syntax/Token.h>



namespace dl7::json::detail {



struct UnescaperErrorHandler
    : public util::Unescaper::ErrorHandler
{
    syntax::Diagnostics* diagnostics;
    const syntax::Token* token;

    UnescaperErrorHandler(syntax::Diagnostics* diagnostics, const syntax::Token* token)
        : util::Unescaper::ErrorHandler(diagnostics->get_log_context())
        , diagnostics(diagnostics)
        , token(token)
    {
    }

    void on_invalid_utf8_encoding(cl7::u8string_view sequence) const override;
    void on_invalid_utf16_encoding(cl7::u16string_view sequence) const override;
}; // struct UnescaperErrorHandler



} // namespace dl7::json::detail

#endif // DL7_JSON_DETAIL_UNESCAPERERRORHANDLER_H
