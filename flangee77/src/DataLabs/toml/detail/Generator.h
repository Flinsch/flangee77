#ifndef DL7_TOML_DETAIL_GENERATOR_H
#define DL7_TOML_DETAIL_GENERATOR_H
#include <DataLabs/syntax/AstProcessor.h>

#include "../Toml.h"
#include "../Format.h"

#include <CoreLabs/sstream.h>



namespace dl7::toml::detail {



class Generator
    : public syntax::AstProcessor<Toml, cl7::u8string>
{

public:
    explicit Generator(syntax::Diagnostics* diagnostics, const Format& format);



    cl7::u8string process(const Toml& toml) override;



private:
    void _write_table(cl7::u8osstream& oss, const table_t& table, cl7::u8string_view name_prefix) const;
    void _write_subtable(cl7::u8osstream& oss, cl7::u8string_view key, const table_t& sub_table, cl7::u8string_view name_prefix) const;
    void _write_array_of_tables(cl7::u8osstream& oss, cl7::u8string_view key, const array_t& array, cl7::u8string_view name_prefix) const;
    void _write_key_value(cl7::u8osstream& oss, cl7::u8string_view key, const Value& value) const;

    cl7::u8osstream& _write_key(cl7::u8osstream& oss, cl7::u8string_view key) const;
    cl7::u8osstream& _write_value(cl7::u8osstream& oss, const Value& value) const;
    cl7::u8osstream& _write_string(cl7::u8osstream& oss, cl7::u8string_view string) const;

    cl7::u8osstream& _write_newline(cl7::u8osstream& oss) const;

    static bool _is_bare_key(cl7::u8string_view key);
    static bool _is_array_of_tables(const Value& value);

    Format _format;

}; // class Generator



} // namespace dl7::toml::detail

#endif // DL7_TOML_DETAIL_GENERATOR_H
