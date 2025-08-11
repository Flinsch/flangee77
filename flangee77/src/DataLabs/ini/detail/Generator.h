#ifndef DL7_INI_DETAIL_GENERATOR_H
#define DL7_INI_DETAIL_GENERATOR_H
#include <DataLabs/syntax/AstProcessor.h>

#include "../Ini.h"
#include "../Format.h"

#include <CoreLabs/sstream.h>



namespace dl7::ini::detail {



class Generator
    : public syntax::AstProcessor<Ini, cl7::u8string>
{

public:
    explicit Generator(syntax::Diagnostics* diagnostics, const Format& format);



    cl7::u8string process(const Ini& root) override;



private:
    void _write_unnamed_section(cl7::u8osstream& oss, const Section& section, cl7::u8string_view section_name_prefix) const;

    void _write_subsections(cl7::u8osstream& oss, const Section::sections_t& sections, cl7::u8string_view section_name_prefix) const;
    void _write_properties(cl7::u8osstream& oss, const Section::properties_t& properties) const;

    cl7::u8osstream& _write_section_name(cl7::u8osstream& oss, cl7::u8string_view string) const;
    cl7::u8osstream& _write_key(cl7::u8osstream& oss, cl7::u8string_view string) const;
    cl7::u8osstream& _write_value(cl7::u8osstream& oss, const Value& value) const;

    cl7::u8osstream& _write_string_value(cl7::u8osstream& oss, cl7::u8string_view string) const;
    cl7::u8osstream& _write_decimal_value(cl7::u8osstream& oss, decimal_t decimal) const;

    cl7::u8osstream& _write_string(cl7::u8osstream& oss, cl7::u8string_view string, cl7::u8string_view invalid_chars) const;
    cl7::u8osstream& _write_quoted_string(cl7::u8osstream& oss, cl7::u8string_view string) const;
    cl7::u8osstream& _write_quoted_string(cl7::u8osstream& oss, cl7::u8string_view string, cl7::u8char_t quote_char) const;
    cl7::u8osstream& _write_unquoted_string(cl7::u8osstream& oss, cl7::u8string_view string) const;

    cl7::u8osstream& _write_newline(cl7::u8osstream& oss) const;

    Format _format;

}; // class Generator



} // namespace dl7::ini::detail

#endif // DL7_INI_DETAIL_GENERATOR_H
