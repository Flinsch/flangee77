#ifndef DL7_YAML_DETAIL_BUILDER_H
#define DL7_YAML_DETAIL_BUILDER_H
#include <DataLabs/syntax/DirectAstBuilder.h>

#include "../Yaml.h"



namespace dl7::yaml::detail {



class Builder
    : public syntax::DirectAstBuilder<Yaml>
{

public:
    explicit Builder(syntax::Diagnostics* diagnostics);



    std::unique_ptr<Yaml> build(syntax::TokenReader& token_reader) override;



private:

}; // class Builder



} // namespace dl7::yaml::detail

#endif // DL7_YAML_DETAIL_BUILDER_H
