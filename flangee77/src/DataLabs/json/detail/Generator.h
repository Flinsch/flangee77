#ifndef DL7_JSON_DETAIL_GENERATOR_H
#define DL7_JSON_DETAIL_GENERATOR_H
#include <DataLabs/syntax/AstProcessor.h>

#include "../Json.h"
#include "../Format.h"

#include <CoreLabs/sstream.h>



namespace dl7::json::detail {



class Generator
    : public syntax::AstProcessor<Json, cl7::u8string>
{

public:
    explicit Generator(syntax::Diagnostics* diagnostics, const Format& format);



    cl7::u8string process(const Json& root) override;



private:
    cl7::u8osstream& _write_json(cl7::u8osstream& oss, const Json& json, size_t depth) const;

    cl7::u8osstream& _write_string(cl7::u8osstream& oss, cl7::u8string_view string) const;
    cl7::u8osstream& _write_string(cl7::u8osstream& oss, cl7::u8string_view string, cl7::u8char_t quote_char) const;

    cl7::u8osstream& _write_decimal(cl7::u8osstream& oss, decimal_t decimal) const;

    void _start_item(cl7::u8osstream& oss, size_t depth) const;
    void _end_item(cl7::u8osstream& oss, size_t index, size_t count) const;

    void _put_line_ending(cl7::u8osstream& oss) const;

    Format _format;

}; // class Generator



} // namespace dl7::json::detail

#endif // DL7_JSON_DETAIL_GENERATOR_H
