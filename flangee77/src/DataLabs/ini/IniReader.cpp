#include "IniReader.h"

#include "./detail/Lexer.h"
#include "./detail/Builder.h"

#include <DataLabs/syntax/SourceAwareDiagnostics.h>
#include <DataLabs/syntax/LexingTokenReader.h>



namespace dl7::ini {



    /**
     * Parses a UTF-8 encoded INI string and returns an `Ini` object.
     */
    Ini IniReader::parse(cl7::u8string_view source)
    {
        syntax::SourceAwareDiagnostics diagnostics{source};

        detail::Lexer lexer{&diagnostics};
        lexer.init(source);

        syntax::LexingTokenReader token_reader{&lexer};

        detail::Builder builder{&diagnostics};
        auto ini = builder.build(token_reader);

        if (!ini)
            return {};

        return std::move(*ini);
    }



} // namespace dl7::ini
