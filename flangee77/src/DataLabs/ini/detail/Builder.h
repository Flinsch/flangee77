#ifndef DL7_INI_DETAIL_BUILDER_H
#define DL7_INI_DETAIL_BUILDER_H
#include <DataLabs/syntax/DirectAstBuilder.h>

#include "../Ini.h"



namespace dl7::ini::detail {



class Builder
    : public syntax::DirectAstBuilder<Ini>
{

public:
    explicit Builder(syntax::Diagnostics* diagnostics);



    std::unique_ptr<Ini> build(syntax::TokenReader& token_reader) override;



private:
    struct Context
    {
        Section* section;
    };

    void _parse_ini(Context& ctx, syntax::TokenReader& token_reader);

    void _parse_line(Context& ctx, syntax::TokenReader& token_reader);

    void _parse_comment(syntax::TokenReader& token_reader);
    void _parse_section(Context& ctx, syntax::TokenReader& token_reader);
    void _parse_key_value(Context& ctx, syntax::TokenReader& token_reader);

    bool _parse_section_name(Context& ctx, syntax::TokenReader& token_reader);
    string_t _parse_section_name(syntax::TokenReader& token_reader);

    string_t _parse_key(syntax::TokenReader& token_reader);
    Value _parse_value(syntax::TokenReader& token_reader);

    string_t _parse_quoted_string(syntax::TokenReader& token_reader);
    decimal_t _parse_decimal(syntax::TokenReader& token_reader);
    integer_t _parse_integer(syntax::TokenReader& token_reader);
    unsigned_t _parse_unsigned(syntax::TokenReader& token_reader);
    string_t _parse_unquoted_string(syntax::TokenReader& token_reader);

    void _error(cl7::u8string_view message, const syntax::Token& token);

}; // class Builder



} // namespace dl7::ini::detail

#endif // DL7_INI_DETAIL_BUILDER_H
