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
    struct Context
    {
        table_t* root;
        table_t* current;
    };

    void _parse_toml(Context& ctx, syntax::TokenReader& token_reader);

    void _parse_line(Context& ctx, syntax::TokenReader& token_reader);

    void _parse_comment(syntax::TokenReader& token_reader);
    void _parse_table_header(Context& ctx, syntax::TokenReader& token_reader);
    void _parse_assignment(table_t& base, syntax::TokenReader& token_reader);

    string_t _parse_key(syntax::TokenReader& token_reader);
    Value _parse_value(syntax::TokenReader& token_reader);
    Value _parse_bare_value(syntax::TokenReader& token_reader);
    array_t _parse_array(syntax::TokenReader& token_reader);
    table_t _parse_inline_table(syntax::TokenReader& token_reader);

    string_t _parse_quoted_string(syntax::TokenReader& token_reader);
    string_t _parse_multiline_string(syntax::TokenReader& token_reader);
    Value _parse_date_time(syntax::TokenReader& token_reader);

    table_t& _descend(table_t& table, const string_t& key, const syntax::Token& token);
    table_t& _descend_array(table_t& table, const string_t& key, const syntax::Token& token);

    void _skip_inline_whitespace(syntax::TokenReader& token_reader);
    void _skip_whitespace_and_newlines(syntax::TokenReader& token_reader);

    void _error(cl7::u8string_view message, const syntax::Token& token);

}; // class Builder



} // namespace dl7::toml::detail

#endif // DL7_TOML_DETAIL_BUILDER_H
