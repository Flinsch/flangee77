#include "JsonReader.h"

#include "./detail/Lexer.h"
#include "./detail/Builder.h"

#include <DataLabs/syntax/SourceAwareDiagnostics.h>
#include <DataLabs/syntax/LexingTokenReader.h>



namespace dl7::json {



    /**
     * Parses a UTF-8 encoded JSON string and returns a `Json` object.
     */
    Json JsonReader::parse(cl7::u8string_view source)
    {
        syntax::SourceAwareDiagnostics diagnostics{source};

        detail::Lexer lexer{&diagnostics};
        lexer.init(source);

        syntax::LexingTokenReader token_reader{&lexer};

        detail::Builder builder{&diagnostics};
        auto json = builder.build(token_reader);

        if (!json)
            return {};

        return std::move(*json);
    }



} // namespace dl7::json
