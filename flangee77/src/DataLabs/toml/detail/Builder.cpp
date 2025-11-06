#include "Builder.h"

#include "./Symbol.h"

#include <sstream>



namespace dl7::toml::detail {



    Builder::Builder(syntax::Diagnostics* diagnostics)
        : DirectAstBuilder(diagnostics)
    {
    }



    std::unique_ptr<Toml> Builder::build(syntax::TokenReader& token_reader)
    {
        auto toml = std::make_unique<Toml>();



        return toml;
    }



} // namespace dl7::toml::detail
