#include "Builder.h"



namespace dl7::json::detail {



    Builder::Builder(syntax::Diagnostics* diagnostics)
        : DirectAstBuilder(diagnostics)
    {
    }



    std::unique_ptr<Json> Builder::build(syntax::TokenReader& token_reader)
    {
        return std::make_unique<Json>();
    }



} // namespace dl7::json::detail
