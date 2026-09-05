#include "YamlReader.h"

#include "./detail/Lexer.h"
#include "./detail/Builder.h"

#include <DataLabs/syntax/SourceAwareDiagnostics.h>
#include <DataLabs/syntax/LexingTokenReader.h>



namespace dl7::yaml {



    /**
     * Parses a UTF-8 encoded YAML string and returns an `Yaml` object.
     */
    Yaml YamlReader::parse(cl7::u8string_view source)
    {
        syntax::SourceAwareDiagnostics diagnostics{source};

        detail::Lexer lexer{&diagnostics};
        lexer.init(source);

        syntax::LexingTokenReader token_reader{&lexer};

        detail::Builder builder{&diagnostics};
        auto yaml = builder.build(token_reader);

        if (!yaml)
            return {};

        return std::move(*yaml);
    }



} // namespace dl7::yaml
