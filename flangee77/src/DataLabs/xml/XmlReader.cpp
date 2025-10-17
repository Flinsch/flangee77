#include "XmlReader.h"

#include "./detail/Lexer.h"
#include "./detail/Builder.h"

#include <DataLabs/syntax/SourceAwareDiagnostics.h>
#include <DataLabs/syntax/LexingTokenReader.h>



namespace dl7::xml {



    /**
     * Parses a UTF-8 encoded XML string and returns a `Document` object.
     */
    Document XmlReader::parse(cl7::u8string_view source)
    {
        syntax::SourceAwareDiagnostics diagnostics{source};

        detail::Lexer lexer{&diagnostics};
        lexer.init(source);

        syntax::LexingTokenReader token_reader{&lexer};

        detail::Builder builder{&diagnostics};
        auto document = builder.build(token_reader);

        if (!document)
            return {};

        return std::move(*document);
    }



} // namespace dl7::xml
