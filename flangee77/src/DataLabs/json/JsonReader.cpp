#include "JsonReader.h"

#include "./detail/Lexer.h"
#include "./detail/Builder.h"

#include <DataLabs/syntax/SourceAwareDiagnostics.h>
#include <DataLabs/syntax/LexingTokenReader.h>



namespace dl7::json {



    /**
     * Parses a UTF-8 encoded JSON string and returns a `Json` object. Whatever
     * there is to complain about ends up in the diagnostics, which are cleared
     * beforehand.
     */
    Json JsonReader::parse(cl7::u8string_view source)
    {
        _diagnostics.clear();

        // Diagnostics that know the source text resolve every entry's context right
        // away. Only they hold a view of it, the finished entries do not. Which is
        // what makes handing those entries over outlive this function safely.
        syntax::SourceAwareDiagnostics source_aware_diagnostics{source};

        detail::Lexer lexer{&source_aware_diagnostics};
        lexer.init(source);

        syntax::LexingTokenReader token_reader{&lexer};

        detail::Builder builder{&source_aware_diagnostics};
        auto json = builder.build(token_reader);

        _diagnostics.add_all(source_aware_diagnostics);

        if (!json)
            return {};

        return std::move(*json);
    }



} // namespace dl7::json
