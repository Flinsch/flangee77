#include "Builder.h"

#include "./Symbol.h"



namespace dl7::xml::detail {



    Builder::Builder(syntax::Diagnostics* diagnostics)
        : DirectAstBuilder(diagnostics)
    {
    }



    std::unique_ptr<Document> Builder::build(syntax::TokenReader& token_reader)
    {
        return {};
    }



} // namespace dl7::xml::detail
