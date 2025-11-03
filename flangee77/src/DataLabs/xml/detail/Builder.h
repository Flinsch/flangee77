#ifndef DL7_XML_DETAIL_BUILDER_H
#define DL7_XML_DETAIL_BUILDER_H
#include <DataLabs/syntax/DirectAstBuilder.h>

#include "../Document.h"
#include "../WhitespaceHandling.h"



namespace dl7::xml::detail {



class Builder
    : public syntax::DirectAstBuilder<Document>
{

public:
    explicit Builder(syntax::Diagnostics* diagnostics, WhitespaceHandling whitespace_handling);



    std::unique_ptr<Document> build(syntax::TokenReader& token_reader) override;



private:
    std::unique_ptr<Document> _parse_document(syntax::TokenReader& token_reader);

    bool _parse_prolog(syntax::TokenReader& token_reader);
    bool _parse_doctype(syntax::TokenReader& token_reader);

    Element _parse_element(syntax::TokenReader& token_reader, WhitespaceHandling whitespace_handling);

    Attribute _parse_attribute(syntax::TokenReader& token_reader);
    bool _parse_equal_sign(syntax::TokenReader& token_reader);
    cl7::u8string _parse_attribute_value(syntax::TokenReader& token_reader);

    std::vector<std::unique_ptr<Node>> _parse_content(syntax::TokenReader& token_reader, WhitespaceHandling whitespace_handling);
    cl7::u8string _parse_char_data(syntax::TokenReader& token_reader, WhitespaceHandling whitespace_handling);
    cl7::u8string _parse_cdata_section(syntax::TokenReader& token_reader);

    cl7::u8string_view _parse_name(syntax::TokenReader& token_reader);
    std::vector<Attribute> _parse_attributes(syntax::TokenReader& token_reader);

    bool _skip_misc(syntax::TokenReader& token_reader, bool skip_whitespace = true);

    void _error(cl7::u8string_view message, const syntax::Token& token);
    void _warning(cl7::u8string_view message, const syntax::Token& token);

    WhitespaceHandling _whitespace_handling;

}; // class Builder



} // namespace dl7::xml::detail

#endif // DL7_XML_DETAIL_BUILDER_H
