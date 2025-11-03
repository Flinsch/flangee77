#ifndef DL7_XML_DETAIL_GENERATOR_H
#define DL7_XML_DETAIL_GENERATOR_H
#include <DataLabs/syntax/AstProcessor.h>

#include "../Document.h"
#include "../Format.h"

#include <CoreLabs/sstream.h>



namespace dl7::xml::detail {



class Generator
    : public syntax::AstProcessor<Document, cl7::u8string>
{

public:
    explicit Generator(syntax::Diagnostics* diagnostics, const Format& format);



    cl7::u8string process(const Document& document) override;



private:
    void _write_element(cl7::u8osstream& oss, const Element& element, unsigned indent_level) const;
    void _write_element_content(cl7::u8osstream& oss, const Element& element, unsigned indent_level) const;
    void _write_text(cl7::u8osstream& oss, const Text& text, unsigned indent_level) const;

    cl7::u8osstream& _write_attribute_value(cl7::u8osstream& oss, cl7::u8string_view value) const;

    cl7::u8osstream& _try_write_indentation(cl7::u8osstream& oss, unsigned indent_level) const;
    cl7::u8osstream& _try_write_newline(cl7::u8osstream& oss) const;
    cl7::u8osstream& _write_newline(cl7::u8osstream& oss) const;

    Format _format;

}; // class Generator



} // namespace dl7::xml::detail

#endif // DL7_XML_DETAIL_GENERATOR_H
