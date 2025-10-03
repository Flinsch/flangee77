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



private:
    std::unique_ptr<Json> _parse_value(syntax::TokenReader& token_reader);

    object_t _parse_object(syntax::TokenReader& token_reader);
    array_t _parse_array(syntax::TokenReader& token_reader);
    string_t _parse_string(syntax::TokenReader& token_reader);
    float_t _parse_float(syntax::TokenReader& token_reader);
    integer_t _parse_integer(syntax::TokenReader& token_reader);

    std::pair<string_t, std::unique_ptr<Json>> _parse_object_member(syntax::TokenReader& token_reader);
    string_t _parse_object_key(syntax::TokenReader& token_reader);
    string_t _parse_identifier(syntax::TokenReader& token_reader);

    void _error(cl7::u8string_view message, const syntax::Token& token);

}; // class Builder



} // namespace dl7::json::detail

#endif // DL7_JSON_DETAIL_BUILDER_H
