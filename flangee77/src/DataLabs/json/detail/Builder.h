#ifndef DL7_JSON_DETAIL_BUILDER_H
#define DL7_JSON_DETAIL_BUILDER_H
#include <DataLabs/syntax/DirectAstBuilder.h>

#include "../Json.h"



namespace dl7::json::detail {



class Builder
    : public syntax::DirectAstBuilder<Json>
{

public:
    explicit Builder(syntax::Diagnostics* diagnostics);



    std::unique_ptr<Json> build(syntax::TokenReader& token_reader) override;

}; // class Builder



} // namespace dl7::json::detail

#endif // DL7_JSON_DETAIL_BUILDER_H
