#ifndef F77_TESTS_DL7_JSON_H
#define F77_TESTS_DL7_JSON_H

#include <DataLabs/json/Json.h>
#include <DataLabs/json/JsonReader.h>
#include <DataLabs/json/JsonWriter.h>
#include <DataLabs/json/util/Validator.h>
#include <DataLabs/json/util/Escaper.h>
#include <DataLabs/json/util/Unescaper.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const dl7::json::Json& json) { return json.to_string(); }
}



TESTLABS_CASE( u8"DataLabs:  json:  Json:  null" )
{
    dl7::json::Json json;

    TESTLABS_CHECK( json.get_type() == dl7::json::Json::Type::Null );
    TESTLABS_CHECK( json.is_null() );
    TESTLABS_CHECK( json.is_primitive() );
    TESTLABS_CHECK_EQ( json.to_string(), u8"null" );
}

TESTLABS_CASE( u8"DataLabs:  json:  Json:  object" )
{
    dl7::json::Json json( dl7::json::Json::Type::Object );

    TESTLABS_CHECK( json.get_type() == dl7::json::Json::Type::Object );
    TESTLABS_CHECK( json.is_object() );
    TESTLABS_CHECK( json.is_structured() );
    TESTLABS_CHECK_EQ( json.to_string(), u8"{}" );
}

TESTLABS_CASE( u8"DataLabs:  json:  Json:  array" )
{
    dl7::json::Json json( dl7::json::Json::Type::Array );

    TESTLABS_CHECK( json.get_type() == dl7::json::Json::Type::Array );
    TESTLABS_CHECK( json.is_array() );
    TESTLABS_CHECK( json.is_structured() );
    TESTLABS_CHECK_EQ( json.to_string(), u8"[]" );
}

TESTLABS_CASE( u8"DataLabs:  json:  Json:  string" )
{
    dl7::json::Json json( u8"Hello World" );

    TESTLABS_CHECK( json.get_type() == dl7::json::Json::Type::String );
    TESTLABS_CHECK( json.is_string() );
    TESTLABS_CHECK( json.is_primitive() );
    TESTLABS_CHECK_EQ( json.to_string(), u8"\"Hello World\"" );
}

TESTLABS_CASE( u8"DataLabs:  json:  Json:  decimal" )
{
    dl7::json::Json json( 7.0 );

    TESTLABS_CHECK( json.get_type() == dl7::json::Json::Type::Float );
    TESTLABS_CHECK( json.is_float() );
    TESTLABS_CHECK( json.is_number() );
    TESTLABS_CHECK( json.is_primitive() );
    TESTLABS_CHECK_EQ( json.to_string(), u8"7.0" );
}

TESTLABS_CASE( u8"DataLabs:  json:  Json:  integer" )
{
    dl7::json::Json json( -7 );

    TESTLABS_CHECK( json.get_type() == dl7::json::Json::Type::Integer );
    TESTLABS_CHECK( json.is_integer() );
    TESTLABS_CHECK( json.is_number() );
    TESTLABS_CHECK( json.is_primitive() );
    TESTLABS_CHECK_EQ( json.to_string(), u8"-7" );
}

TESTLABS_CASE( u8"DataLabs:  json:  Json:  true" )
{
    dl7::json::Json json( true );

    TESTLABS_CHECK( json.get_type() == dl7::json::Json::Type::Boolean );
    TESTLABS_CHECK( json.is_boolean() );
    TESTLABS_CHECK( json.is_true() );
    TESTLABS_CHECK( json.is_primitive() );
    TESTLABS_CHECK_EQ( json.to_string(), u8"true" );
}

TESTLABS_CASE( u8"DataLabs:  json:  Json:  false" )
{
    dl7::json::Json json( false );

    TESTLABS_CHECK( json.get_type() == dl7::json::Json::Type::Boolean );
    TESTLABS_CHECK( json.is_boolean() );
    TESTLABS_CHECK( json.is_false() );
    TESTLABS_CHECK( json.is_primitive() );
    TESTLABS_CHECK_EQ( json.to_string(), u8"false" );
}



TESTLABS_CASE( u8"DataLabs:  json:  util::Validator::is_valid_unquoted_key" )
{
    struct Entry
    {
        cl7::u8string key;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", false },

        // https://tc39.es/ecma262/#prod-ReservedWord
        { u8"await", false },
        { u8"break", false },
        { u8"case", false },
        { u8"catch", false },
        { u8"class", false },
        { u8"const", false },
        { u8"continue", false },
        { u8"debugger", false },
        { u8"default", false },
        { u8"delete", false },
        { u8"do", false },
        { u8"else", false },
        { u8"enum", false },
        { u8"export", false },
        { u8"extends", false },
        { u8"false", false },
        { u8"finally", false },
        { u8"for", false },
        { u8"function", false },
        { u8"if", false },
        { u8"import", false },
        { u8"in", false },
        { u8"instanceof", false },
        { u8"new", false },
        { u8"null", false },
        { u8"return", false },
        { u8"super", false },
        { u8"switch", false },
        { u8"this", false },
        { u8"throw", false },
        { u8"true", false },
        { u8"try", false },
        { u8"typeof", false },
        { u8"var", false },
        { u8"void", false },
        { u8"while", false },
        { u8"with", false },
        { u8"yield", false },

        { u8"validKey", true },
        { u8"valid_key", true },
        { u8"_validKey", true },
        { u8"_valid_key", true },
        { u8"$validKey", true },
        { u8"$valid_key", true },
        { u8"_", true },
        { u8"$", true },
        { u8"__", true },
        { u8"$$", true },
        { u8"_$", true },
        { u8"$_", true },
        { u8"abc123", true },

        { u8"invalid key", false },
        { u8" invalidKey", false },
        { u8"invalid-key", false },
        { u8"1 invalid key", false },
        { u8"1_invalid_key", false },
        { u8"ungültigerSchlüssel", false },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.key )
    {
        TESTLABS_CHECK_EQ( dl7::json::util::Validator::is_valid_unquoted_key( entry.key ), entry.expected );
    }
}



TESTLABS_CASE( u8"DataLabs:  json:  util::Escaper::escape_string" )
{
    struct Entry
    {
        cl7::u8string raw_string;
        dl7::json::util::Escaper::Options options;
        cl7::u8string expected_string;
    } entry;

    const std::vector<Entry> container {
        { u8"", {}, u8"" },
        { u8"Hello World", {}, u8"Hello World" },
        { u8"Hello\nWorld", {}, u8"Hello\\nWorld" },
        { u8"Quote: \"", { dl7::json::util::Escaper::QuoteChar::DoubleQuote }, u8"Quote: \\\"" },
        { u8"Quote: '", { dl7::json::util::Escaper::QuoteChar::DoubleQuote }, u8"Quote: '" },
        { u8"Quote: \"", { dl7::json::util::Escaper::QuoteChar::SingleQuote }, u8"Quote: \"" },
        { u8"Quote: '", { dl7::json::util::Escaper::QuoteChar::SingleQuote }, u8"Quote: \\'" },
        { u8"\x01", {}, u8"\\u0001" },
        { u8"\U0001f610", { {}, false }, u8"\U0001f610" },
        { u8"\U0001f610", { {}, true }, u8"\\uD83D\\uDE10" },
        { u8"\x07\x08\x09\x0a\x0b\x0c\x0d\x1a\x1b/\\", {}, u8"\\u0007\\b\\t\\n\\u000B\\f\\r\\u001A\\u001B/\\\\" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.raw_string )
    {
        TESTLABS_CHECK_EQ( dl7::json::util::Escaper{}.escape_string( entry.raw_string, entry.options ), entry.expected_string );
    }
}



TESTLABS_CASE( u8"DataLabs:  json:  util::Unescaper::unescape_string" )
{
    struct Entry
    {
        cl7::u8string json_string;
        cl7::u8string expected_string;
        bool expected_result = true;
    } entry;

    const std::vector<Entry> container {
        { u8"", u8"" },
        { u8"Hello World", u8"Hello World" },
        { u8"Hello\\nWorld", u8"Hello\nWorld" },
        { u8"Quote: \\\"", u8"Quote: \"" },
        { u8"Quote: '", u8"Quote: '" },
        { u8"Quote: \"", u8"Quote: \"" },
        { u8"Quote: \\'", u8"Quote: '" },
        { u8"\\u0001", u8"\x01" },
        { u8"\U0001f610", u8"\U0001f610" },
        { u8"\\uD83D\\uDE10", u8"\U0001f610" },
        { u8"\\x07\\b\\t\\n\\v\\f\\r\\u001A\\u001B/\\/\\\\", u8"\x07\x08\x09\x0a\x0b\x0c\x0d\x1a\x1b//\\" },
        { u8"a\\x61\\xF0\\x9f\\x98\\x90\\x61a", u8"aa\U0001f610aa", true },
        { u8"a\\x61\\x9F\\xF0\\x98\\x90\\x61a", u8"aa\ufffd\ufffdaa", false },
        { u8"\\", u8"\\", false },
        { u8"\\uDE10\\uD83D", u8"\ufffd\ufffd", false },
        { u8"\\u123", u8"\\u123", false },
        { u8"\\x1", u8"\\x1", false },
        { u8"\\yz", u8"\\yz", false },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.json_string )
    {
        TESTLABS_CHECK_EQ( dl7::json::util::Unescaper{}.unescape_string( entry.json_string ), entry.expected_string );
        cl7::u8osstream oss;
        TESTLABS_CHECK_EQ( dl7::json::util::Unescaper{}.unescape_string( oss, entry.json_string ), entry.expected_result );
    }
}



TESTLABS_CASE( u8"DataLabs:  json:  JsonReader:  parse" )
{
    struct Entry
    {
        cl7::u8string string;
        dl7::json::Json json;
    } entry;

    const std::vector<Entry> container {
        { u8"null", dl7::json::Json( dl7::json::Json::Type::Null ) },
        { u8"{}", dl7::json::Json( dl7::json::Json::Type::Object ) },
        { u8"[]", dl7::json::Json( dl7::json::Json::Type::Array ) },
        { u8"\"Hello World\"", dl7::json::Json( u8"Hello World" ) },
        { u8"7.0", dl7::json::Json( 7.0 ) },
        { u8"-7", dl7::json::Json( -7 ) },
        { u8"7", dl7::json::Json( 7ul ) },
        { u8"true", dl7::json::Json( true ) },
        { u8"false", dl7::json::Json( false ) },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.string )
    {
        const auto json = dl7::json::JsonReader::parse( entry.string );
        TESTLABS_CHECK_EQ( json, entry.json );
    }
}



TESTLABS_CASE( u8"DataLabs:  json:  JsonWriter:  to_string" )
{
    struct Entry
    {
        dl7::json::Json json;
        dl7::json::Format format;
        cl7::u8string string;
    } entry;

    const std::vector<Entry> container {
        { {}, dl7::json::JsonWriter::DEFAULT_FORMAT, u8"null" },
        { dl7::json::Json{ dl7::json::Json::Type::Object }, dl7::json::JsonWriter::DEFAULT_FORMAT, u8"{}" },
        { dl7::json::Json{ dl7::json::Json::Type::Array }, dl7::json::JsonWriter::DEFAULT_FORMAT, u8"[]" },
        { dl7::json::Json{ u8"Hello World" }, dl7::json::JsonWriter::DEFAULT_FORMAT, u8"\"Hello World\"" },
        { dl7::json::Json{ 7.0 }, dl7::json::JsonWriter::DEFAULT_FORMAT, u8"7.0" },
        { dl7::json::Json{ -7 }, dl7::json::JsonWriter::DEFAULT_FORMAT, u8"-7" },
        { dl7::json::Json{ 7ul }, dl7::json::JsonWriter::DEFAULT_FORMAT, u8"7" },
        { dl7::json::Json{ true }, dl7::json::JsonWriter::DEFAULT_FORMAT, u8"true" },
        { dl7::json::Json{ false }, dl7::json::JsonWriter::DEFAULT_FORMAT, u8"false" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.string )
    {
        TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( entry.json, entry.format ), entry.string );
    }
}



TESTLABS_CASE( u8"DataLabs:  json:  JsonReader / JsonWriter" )
{
    struct Entry
    {
        cl7::u8string string;
        dl7::json::Format format;
    } entry;

    const std::vector<Entry> container {
        { u8"", dl7::json::JsonWriter::DEFAULT_FORMAT },
        { u8"null", dl7::json::JsonWriter::DEFAULT_FORMAT },
        { u8"true", dl7::json::JsonWriter::DEFAULT_FORMAT },
        { u8"false", dl7::json::JsonWriter::DEFAULT_FORMAT },
        { u8"-7", dl7::json::JsonWriter::DEFAULT_FORMAT },
        { u8"7", dl7::json::JsonWriter::DEFAULT_FORMAT },
        { u8"7.0", dl7::json::JsonWriter::DEFAULT_FORMAT },
        { u8"7.0e+10", dl7::json::JsonWriter::DEFAULT_FORMAT },
        { u8"\"Hello World\"", dl7::json::JsonWriter::DEFAULT_FORMAT },
        { u8"[]", dl7::json::JsonWriter::DEFAULT_FORMAT },
        { u8"{}", dl7::json::JsonWriter::DEFAULT_FORMAT },
        {
u8"{\n"
"    \"first_name\": \"John\",\n"
"    \"last_name\": \"Smith\",\n"
"    \"full_name\": \"John \\\"Pseudo\\\" Smith\",\n"
"    \"is_alive\": true,\n"
"    \"age\": 27,\n"
"    \"address\": {\n"
"        \"street_address\": \"21 2nd Street\",\n"
"        \"city\": \"New York\",\n"
"        \"state\": \"NY\",\n"
"        \"postal_code\": \"10021-3100\"\n"
"    },\n"
"    \"phone_numbers\": [\n"
"        {\n"
"            \"type\": \"home\",\n"
"            \"number\": \"212 555-1234\"\n"
"        },\n"
"        {\n"
"            \"type\": \"office\",\n"
"            \"number\": \"646 555-4567\"\n"
"        }\n"
"    ],\n"
"    \"children\": [\n"
"        \"Catherine\",\n"
"        \"Thomas\",\n"
"        \"Trevor\"\n"
"    ],\n"
"    \"spouse\": null,\n"
"    \"emoji\": \"\U0001f610\",\n"
"    \"is_confirmed\": false\n"
"}",
            dl7::json::JsonWriter::DEFAULT_MULTI_LINE_FORMAT,
        },
        {
u8"{\n"
"    \"first_name\": \"John\",\n"
"    \"last_name\": \"Smith\",\n"
"    \"full_name\": \"John \\\"Pseudo\\\" Smith\",\n"
"    \"is_alive\": true,\n"
"    \"age\": 27,\n"
"    \"address\": {\n"
"        \"street_address\": \"21 2nd Street\",\n"
"        \"city\": \"New York\",\n"
"        \"state\": \"NY\",\n"
"        \"postal_code\": \"10021-3100\",\n"
"    },\n"
"    \"phone_numbers\": [\n"
"        {\n"
"            \"type\": \"home\",\n"
"            \"number\": \"212 555-1234\",\n"
"        },\n"
"        {\n"
"            \"type\": \"office\",\n"
"            \"number\": \"646 555-4567\",\n"
"        },\n"
"    ],\n"
"    \"children\": [\n"
"        \"Catherine\",\n"
"        \"Thomas\",\n"
"        \"Trevor\",\n"
"    ],\n"
"    \"spouse\": null,\n"
"    \"emoji\": \"\U0001f610\",\n"
"    \"is_confirmed\": false,\n"
"}\n",
            dl7::json::JsonWriter::DEFAULT_PRETTY_PRINT_FORMAT,
        },
        {
u8"{"
"\"first_name\": \"John\", "
"\"last_name\": \"Smith\", "
"\"full_name\": \"John \\\"Pseudo\\\" Smith\", "
"\"is_alive\": true, "
"\"age\": 27, "
"\"address\": {"
"\"street_address\": \"21 2nd Street\", "
"\"city\": \"New York\", "
"\"state\": \"NY\", "
"\"postal_code\": \"10021-3100\""
"}, "
"\"phone_numbers\": ["
"{"
"\"type\": \"home\", "
"\"number\": \"212 555-1234\""
"}, "
"{"
"\"type\": \"office\", "
"\"number\": \"646 555-4567\""
"}"
"], "
"\"children\": ["
"\"Catherine\", "
"\"Thomas\", "
"\"Trevor\""
"], "
"\"spouse\": null, "
"\"emoji\": \"\U0001f610\", "
"\"is_confirmed\": false"
"}",
            dl7::json::JsonWriter::DEFAULT_SINGLE_LINE_FORMAT,
        },
        {
u8"{"
"\"first_name\":\"John\","
"\"last_name\":\"Smith\","
"\"full_name\":\"John \\\"Pseudo\\\" Smith\","
"\"is_alive\":true,"
"\"age\":27,"
"\"address\":{"
"\"street_address\":\"21 2nd Street\","
"\"city\":\"New York\","
"\"state\":\"NY\","
"\"postal_code\":\"10021-3100\""
"},"
"\"phone_numbers\":["
"{"
"\"type\":\"home\","
"\"number\":\"212 555-1234\""
"},"
"{"
"\"type\":\"office\","
"\"number\":\"646 555-4567\""
"}"
"],"
"\"children\":["
"\"Catherine\","
"\"Thomas\","
"\"Trevor\""
"],"
"\"spouse\":null,"
"\"emoji\":\"\U0001f610\","
"\"is_confirmed\":false"
"}",
            dl7::json::JsonWriter::DEFAULT_COMPACT_FORMAT,
        },
        {
u8"{\r"
"  first_name: \"John\",\r"
"  last_name: \"Smith\",\r"
"  full_name: 'John \"Pseudo\" Smith',\r"
"  \"foo-bar\": [\r"
"    \"foo\",\r"
"    \"bar\"\r"
"  ],\r"
"  emoji: \"\\uD83D\\uDE10\",\r"
"  is_confirmed: false\r"
"}",
            {.style=dl7::json::Format::Style::MultiLine, .single_line_options={.compact=false}, .multi_line_options={.indentation=dl7::json::Format::MultiLineOptions::Indentation::Spaces2, .line_ending=dl7::json::Format::MultiLineOptions::LineEnding::CR, .add_trailing_commas=false, .add_empty_line=false}, .escape_unicode=true, .allow_single_quotes=true, .allow_unquoted_keys=true, .float_policy=dl7::json::Format::FloatPolicy::ReplaceWithNull},
        },
        {
u8"{\r\n"
"\tfirst_name: \"John\",\r\n"
"\tlast_name: \"Smith\",\r\n"
"\tfull_name: 'John \"Pseudo\" Smith',\r\n"
"\t\"foo-bar\": [\r\n"
"\t\t\"foo\",\r\n"
"\t\t\"bar\"\r\n"
"\t],\r\n"
"\temoji: \"\\uD83D\\uDE10\",\r\n"
"\tis_confirmed: false\r\n"
"}",
            {.style=dl7::json::Format::Style::MultiLine, .single_line_options={.compact=false}, .multi_line_options={.indentation=dl7::json::Format::MultiLineOptions::Indentation::Tabs, .line_ending=dl7::json::Format::MultiLineOptions::LineEnding::CRLF, .add_trailing_commas=false, .add_empty_line=false}, .escape_unicode=true, .allow_single_quotes=true, .allow_unquoted_keys=true, .float_policy=dl7::json::Format::FloatPolicy::ReplaceWithNull},
        },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.string )
    {
        const auto json = dl7::json::JsonReader::parse( entry.string );
        if (entry.string.empty())
            TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( json, entry.format ), u8"null" );
        else
            TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( json, entry.format ), entry.string );
    }
}

TESTLABS_CASE( u8"DataLabs:  json:  JsonWriter (NaN and infinite values)" )
{
    dl7::json::Format default_format = dl7::json::JsonWriter::DEFAULT_FORMAT;

    dl7::json::Format format_replace_with_null = default_format;
    format_replace_with_null.float_policy = dl7::json::Format::FloatPolicy::ReplaceWithNull;

    dl7::json::Format format_replace_with_zero = default_format;
    format_replace_with_zero.float_policy = dl7::json::Format::FloatPolicy::ReplaceWithZero;

    dl7::json::Format format_encode_as_string = default_format;
    format_encode_as_string.float_policy = dl7::json::Format::FloatPolicy::EncodeAsString;

    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( std::numeric_limits<float>::quiet_NaN() ), default_format ), u8"null" );
    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( +std::numeric_limits<float>::infinity() ), default_format ), u8"null" );
    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( -std::numeric_limits<float>::infinity() ), default_format ), u8"null" );

    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( std::numeric_limits<float>::quiet_NaN() ), format_replace_with_null ), u8"null" );
    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( +std::numeric_limits<float>::infinity() ), format_replace_with_null ), u8"null" );
    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( -std::numeric_limits<float>::infinity() ), format_replace_with_null ), u8"null" );

    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( std::numeric_limits<float>::quiet_NaN() ), format_replace_with_zero ), u8"0.0" );
    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( +std::numeric_limits<float>::infinity() ), format_replace_with_zero ), u8"0.0" );
    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( -std::numeric_limits<float>::infinity() ), format_replace_with_zero ), u8"0.0" );

    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( std::numeric_limits<float>::quiet_NaN() ), format_encode_as_string ), u8"\"NaN\"" );
    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( +std::numeric_limits<float>::infinity() ), format_encode_as_string ), u8"\"Infinity\"" );
    TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( dl7::json::Json( -std::numeric_limits<float>::infinity() ), format_encode_as_string ), u8"\"-Infinity\"" );
}



#endif // F77_TESTS_DL7_JSON_H
