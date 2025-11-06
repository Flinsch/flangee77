#include "TomlReader.h"

#include "./detail/Lexer.h"
#include "./detail/Builder.h"

#include <DataLabs/syntax/SourceAwareDiagnostics.h>
#include <DataLabs/syntax/LexingTokenReader.h>



namespace dl7::toml {



    /**
     * Parses a UTF-8 encoded TOML string and returns an `Toml` object.
     */
    Toml TomlReader::parse(cl7::u8string_view source)
    {
        syntax::SourceAwareDiagnostics diagnostics{source};

        detail::Lexer lexer{&diagnostics};
        lexer.init(source);

        syntax::LexingTokenReader token_reader{&lexer};

        detail::Builder builder{&diagnostics};
        auto toml = builder.build(token_reader);

        if (!toml)
            return {};

        return std::move(*toml);
    }



} // namespace dl7::toml
