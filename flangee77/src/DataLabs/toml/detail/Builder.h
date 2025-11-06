#ifndef DL7_TOML_DETAIL_BUILDER_H
#define DL7_TOML_DETAIL_BUILDER_H
#include <DataLabs/syntax/DirectAstBuilder.h>

#include "../Toml.h"



namespace dl7::toml::detail {



class Builder
    : public syntax::DirectAstBuilder<Toml>
{

public:
    explicit Builder(syntax::Diagnostics* diagnostics);



    std::unique_ptr<Toml> build(syntax::TokenReader& token_reader) override;



private:

}; // class Builder



} // namespace dl7::toml::detail

#endif // DL7_TOML_DETAIL_BUILDER_H
