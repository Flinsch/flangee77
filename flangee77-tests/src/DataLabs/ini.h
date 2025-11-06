#ifndef F77_TESTS_DL7_INI_H
#define F77_TESTS_DL7_INI_H

#include <DataLabs/ini/Ini.h>
#include <DataLabs/ini/IniReader.h>
#include <DataLabs/ini/IniWriter.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const dl7::ini::Ini& ini) { return dl7::ini::IniWriter::to_string( ini ); }
}



TESTLABS_CASE( u8"DataLabs:  ini:  Value:  undefined" )
{
    dl7::ini::Value value;

    TESTLABS_CHECK( value.get_type() == dl7::ini::Value::Type::Undefined );
    TESTLABS_CHECK( value.is_undefined() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"" );
}

TESTLABS_CASE( u8"DataLabs:  ini:  Value:  string" )
{
    dl7::ini::Value value( u8"Hello World" );

    TESTLABS_CHECK( value.get_type() == dl7::ini::Value::Type::String );
    TESTLABS_CHECK( value.is_string() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"Hello World" );
}

TESTLABS_CASE( u8"DataLabs:  ini:  Value:  decimal" )
{
    dl7::ini::Value value( 7.0 );

    TESTLABS_CHECK( value.get_type() == dl7::ini::Value::Type::Float );
    TESTLABS_CHECK( value.is_float() );
    TESTLABS_CHECK( value.is_number() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"7.0" );
}

TESTLABS_CASE( u8"DataLabs:  ini:  Value:  integer" )
{
    dl7::ini::Value value( -7 );

    TESTLABS_CHECK( value.get_type() == dl7::ini::Value::Type::Integer );
    TESTLABS_CHECK( value.is_integer() );
    TESTLABS_CHECK( value.is_number() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"-7" );
}

TESTLABS_CASE( u8"DataLabs:  ini:  Ini:  true" )
{
    dl7::ini::Value value( true );

    TESTLABS_CHECK( value.get_type() == dl7::ini::Value::Type::Boolean );
    TESTLABS_CHECK( value.is_boolean() );
    TESTLABS_CHECK( value.is_true() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"true" );
}

TESTLABS_CASE( u8"DataLabs:  ini:  Ini:  false" )
{
    dl7::ini::Value value( false );

    TESTLABS_CHECK( value.get_type() == dl7::ini::Value::Type::Boolean );
    TESTLABS_CHECK( value.is_boolean() );
    TESTLABS_CHECK( value.is_false() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"false" );
}



TESTLABS_CASE( u8"DataLabs:  ini:  IniReader:  parse" )
{
    struct Entry
    {
        cl7::u8string string;
        dl7::ini::Ini ini;
    } entry;

    const std::vector<Entry> container {
        { u8"", dl7::ini::Ini() },
        { u8"foo=", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value() } } ) },
        { u8"foo=bar", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ) },
        { u8"foo=7.0", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( 7.0f ) } } ) },
        { u8"foo=-7", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( -7 ) } } ) },
        { u8"foo=7", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( 7ul ) } } ) },
        { u8"foo=true", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( true ) } } ) },
        { u8"foo=false", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( false ) } } ) },
        { u8"foo=\"7.0\"", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"7.0" ) } } ) },
        { u8"foo=\"-7\"", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"-7" ) } } ) },
        { u8"foo=\"7\"", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"7" ) } } ) },
        { u8"foo=\"true\"", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"true" ) } } ) },
        { u8"foo=\"false\"", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"false" ) } } ) },
        { u8"foo=bar 7 true", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar 7 true" ) } } ) },
        { u8"foo=7 true bar", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"7 true bar" ) } } ) },
        { u8"foo=true bar 7", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"true bar 7" ) } } ) },
        { u8"[foo.bar]", dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"foo", dl7::ini::Ini::sections_t{ { u8"bar", dl7::ini::Ini::properties_t{} } } } } ) },
        { u8"[=:]", dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"=:", dl7::ini::Ini::properties_t{} } } ) },
        { u8"./=\"\\\"'#;\"", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"./", dl7::ini::Value( u8"\"'#;" ) } } ) },
        { u8"# this is a comment", dl7::ini::Ini() },
        { u8"foo=bar ; this is a comment", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ) },
        { u8"foo=bar; this is not considered a comment", dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar; this is not considered a comment" ) } } ) },
        {
u8"\n"
"global=foobar\n"
"[section]\n"
" foo = bar \n"
"mixed string = prefix \"quoted infix\" suffix \n"
"multi-line string: first line \\\n"
"                   second line \n"
"CRLF = \r\n"
"  LF = \n"
"CR   = \r"
"\"invalid quoted\" = \"A\x81Z\"\n"
"invalid unquoted = A\x81Z\n"
"", dl7::ini::Ini( dl7::ini::Ini::sections_t{
                { u8"section", dl7::ini::Ini::properties_t{ {
                    { u8"foo", dl7::ini::Value( u8"bar" ) },
                    { u8"mixed string", dl7::ini::Value( u8"prefix quoted infix suffix" ) },
                    { u8"multi-line string", dl7::ini::Value( u8"first line second line" ) },
                    { u8"CRLF", dl7::ini::Value() },
                    { u8"LF", dl7::ini::Value() },
                    { u8"CR", dl7::ini::Value() },
                    { u8"invalid quoted", dl7::ini::Value() },
                    { u8"invalid unquoted", dl7::ini::Value( u8"A\ufffdZ" ) },
                } } }
            }, dl7::ini::Ini::properties_t{
                { u8"global", dl7::ini::Value( u8"foobar" ) }
            } )
        },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.string )
    {
        const auto ini = dl7::ini::IniReader::parse( entry.string );
        TESTLABS_CHECK_EQ( ini, entry.ini );
    }
}



TESTLABS_CASE( u8"DataLabs:  ini:  IniWriter:  to_string" )
{
    struct Entry
    {
        dl7::ini::Ini ini;
        dl7::ini::Format format;
        cl7::u8string string;
    } entry;

    dl7::ini::Format format;

    const std::vector<Entry> container {
        { dl7::ini::Ini(), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value() } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=bar\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( 7.0f ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=7.0\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( -7 ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=-7\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( 7ul ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=7\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( true ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=true\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( false ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=false\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"7.0" ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=\"7.0\"\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"-7" ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=\"-7\"\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"7" ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=\"7\"\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"true" ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=\"true\"\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"false" ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"foo=\"false\"\n" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"foo", dl7::ini::Ini::sections_t{ { u8"bar", dl7::ini::Ini::properties_t{} } } } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"[foo.bar]\n" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"./", dl7::ini::Ini::properties_t{} } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"[\"./\"]\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"=:", dl7::ini::Value( u8"\"'#;" ) } } ), dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, u8"\"=:\"=\"\\\"'#;\"\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.line_ending = dl7::ini::Format::LineEnding::CRLF, format), u8"foo=bar\r\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.line_ending = dl7::ini::Format::LineEnding::LF, format), u8"foo=bar\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.line_ending = dl7::ini::Format::LineEnding::CR, format), u8"foo=bar\r" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"foo", dl7::ini::Ini::sections_t{ { u8"bar", dl7::ini::Ini::properties_t{} } } } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.section_name_delimiter = dl7::ini::Format::SectionNameDelimiter::Dot, format), u8"[foo.bar]\n" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"foo", dl7::ini::Ini::sections_t{ { u8"bar", dl7::ini::Ini::properties_t{} } } } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.section_name_delimiter = dl7::ini::Format::SectionNameDelimiter::Slash, format), u8"[foo/bar]\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.key_value_delimiter = dl7::ini::Format::KeyValueDelimiter::Equals, format), u8"foo=bar\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.key_value_delimiter = dl7::ini::Format::KeyValueDelimiter::Colon, format), u8"foo:bar\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.space_around_delimiter = dl7::ini::Format::SpaceAroundKeyValueDelimiter::None, format), u8"foo=bar\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.space_around_delimiter = dl7::ini::Format::SpaceAroundKeyValueDelimiter::Before, format), u8"foo =bar\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.space_around_delimiter = dl7::ini::Format::SpaceAroundKeyValueDelimiter::After, format), u8"foo= bar\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.space_around_delimiter = dl7::ini::Format::SpaceAroundKeyValueDelimiter::Both, format), u8"foo = bar\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"\"bar\"" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.allow_single_quotes = false, format), u8"foo=\"\\\"bar\\\"\"\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"\"bar\"" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.allow_single_quotes = true, format), u8"foo='\"bar\"'\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.empty_line_at_start_of_file = false, format), u8"foo=bar\n" },
        { dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( u8"bar" ) } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.empty_line_at_start_of_file = true, format), u8"\nfoo=bar\n" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"A", dl7::ini::Ini::properties_t{ { u8"A", dl7::ini::Value( u8"A" ) } } }, { u8"B", dl7::ini::Ini::properties_t{ { u8"B", dl7::ini::Value( u8"B" ) } } } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.empty_line_at_start_of_file = false, format.empty_line_before_section_header = false, format.empty_line_after_section_header = false, format), u8"[A]\nA=A\n[B]\nB=B\n" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"A", dl7::ini::Ini::properties_t{ { u8"A", dl7::ini::Value( u8"A" ) } } }, { u8"B", dl7::ini::Ini::properties_t{ { u8"B", dl7::ini::Value( u8"B" ) } } } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.empty_line_at_start_of_file = true, format.empty_line_before_section_header = false, format.empty_line_after_section_header = false, format), u8"\n[A]\nA=A\n[B]\nB=B\n" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"A", dl7::ini::Ini::properties_t{ { u8"A", dl7::ini::Value( u8"A" ) } } }, { u8"B", dl7::ini::Ini::properties_t{ { u8"B", dl7::ini::Value( u8"B" ) } } } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.empty_line_at_start_of_file = false, format.empty_line_before_section_header = true, format.empty_line_after_section_header = false, format), u8"\n[A]\nA=A\n\n[B]\nB=B\n" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"A", dl7::ini::Ini::properties_t{ { u8"A", dl7::ini::Value( u8"A" ) } } }, { u8"B", dl7::ini::Ini::properties_t{ { u8"B", dl7::ini::Value( u8"B" ) } } } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.empty_line_at_start_of_file = true, format.empty_line_before_section_header = true, format.empty_line_after_section_header = false, format), u8"\n[A]\nA=A\n\n[B]\nB=B\n" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"A", dl7::ini::Ini::properties_t{ { u8"A", dl7::ini::Value( u8"A" ) } } }, { u8"B", dl7::ini::Ini::properties_t{ { u8"B", dl7::ini::Value( u8"B" ) } } } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.empty_line_at_start_of_file = false, format.empty_line_before_section_header = false, format.empty_line_after_section_header = true, format), u8"[A]\n\nA=A\n[B]\n\nB=B\n" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"A", dl7::ini::Ini::properties_t{ { u8"A", dl7::ini::Value( u8"A" ) } } }, { u8"B", dl7::ini::Ini::properties_t{ { u8"B", dl7::ini::Value( u8"B" ) } } } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.empty_line_at_start_of_file = true, format.empty_line_before_section_header = false, format.empty_line_after_section_header = true, format), u8"\n[A]\n\nA=A\n[B]\n\nB=B\n" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"A", dl7::ini::Ini::properties_t{ { u8"A", dl7::ini::Value( u8"A" ) } } }, { u8"B", dl7::ini::Ini::properties_t{ { u8"B", dl7::ini::Value( u8"B" ) } } } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.empty_line_at_start_of_file = false, format.empty_line_before_section_header = true, format.empty_line_after_section_header = true, format), u8"\n[A]\n\nA=A\n\n[B]\n\nB=B\n" },
        { dl7::ini::Ini( dl7::ini::Ini::sections_t{ { u8"A", dl7::ini::Ini::properties_t{ { u8"A", dl7::ini::Value( u8"A" ) } } }, { u8"B", dl7::ini::Ini::properties_t{ { u8"B", dl7::ini::Value( u8"B" ) } } } } ), (format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT, format.empty_line_at_start_of_file = true, format.empty_line_before_section_header = true, format.empty_line_after_section_header = true, format), u8"\n[A]\n\nA=A\n\n[B]\n\nB=B\n" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.string )
    {
        const auto string = dl7::ini::IniWriter::to_string( entry.ini, entry.format );
        TESTLABS_CHECK_EQ( string, entry.string );
    }
}

TESTLABS_CASE( u8"DataLabs:  ini:  IniWriter (NaN and infinite values)" )
{
    dl7::ini::Format default_format = dl7::ini::IniWriter::DEFAULT_COMPACT_FORMAT;

    dl7::ini::Format format_replace_with_undefined = default_format;
    format_replace_with_undefined.float_policy = dl7::ini::Format::FloatPolicy::ReplaceWithUndefined;

    dl7::ini::Format format_replace_with_zero = default_format;
    format_replace_with_zero.float_policy = dl7::ini::Format::FloatPolicy::ReplaceWithZero;

    dl7::ini::Format format_encode_as_string = default_format;
    format_encode_as_string.float_policy = dl7::ini::Format::FloatPolicy::EncodeAsString;

    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( std::numeric_limits<float>::quiet_NaN() ) } } ), default_format ), u8"foo=\n" );
    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( +std::numeric_limits<float>::infinity() ) } } ), default_format ), u8"foo=\n" );
    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( -std::numeric_limits<float>::infinity() ) } } ), default_format ), u8"foo=\n" );

    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( std::numeric_limits<float>::quiet_NaN() ) } } ), format_replace_with_undefined ), u8"foo=\n" );
    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( +std::numeric_limits<float>::infinity() ) } } ), format_replace_with_undefined ), u8"foo=\n" );
    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( -std::numeric_limits<float>::infinity() ) } } ), format_replace_with_undefined ), u8"foo=\n" );

    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( std::numeric_limits<float>::quiet_NaN() ) } } ), format_replace_with_zero ), u8"foo=0.0\n" );
    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( +std::numeric_limits<float>::infinity() ) } } ), format_replace_with_zero ), u8"foo=0.0\n" );
    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( -std::numeric_limits<float>::infinity() ) } } ), format_replace_with_zero ), u8"foo=0.0\n" );

    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( std::numeric_limits<float>::quiet_NaN() ) } } ), format_encode_as_string ), u8"foo=\"NaN\"\n" );
    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( +std::numeric_limits<float>::infinity() ) } } ), format_encode_as_string ), u8"foo=\"Infinity\"\n" );
    TESTLABS_CHECK_EQ( dl7::ini::IniWriter::to_string( dl7::ini::Ini( dl7::ini::Ini::properties_t{ { u8"foo", dl7::ini::Value( -std::numeric_limits<float>::infinity() ) } } ), format_encode_as_string ), u8"foo=\"-Infinity\"\n" );
}



#endif // F77_TESTS_DL7_INI_H
