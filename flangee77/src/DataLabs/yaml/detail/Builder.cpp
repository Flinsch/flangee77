#include "Builder.h"

#include "./Symbol.h"

#include <sstream>



namespace dl7::yaml::detail {



    Builder::Builder(syntax::Diagnostics* diagnostics)
        : DirectAstBuilder(diagnostics)
    {
    }



    std::unique_ptr<Yaml> Builder::build(syntax::TokenReader& token_reader)
    {
        auto yaml = std::make_unique<Yaml>();



        return yaml;
    }



} // namespace dl7::yaml::detail
