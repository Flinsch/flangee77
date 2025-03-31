#ifndef F77_TESTS_DL7_JSON_H
#define F77_TESTS_DL7_JSON_H

#include <DataLabs/json/Json.h>
#include <DataLabs/json/JsonWriter.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



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

    TESTLABS_CHECK( json.get_type() == dl7::json::Json::Type::Decimal );
    TESTLABS_CHECK( json.is_decimal() );
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

TESTLABS_CASE( u8"DataLabs:  json:  Json:  unsigned" )
{
    dl7::json::Json json( 7ul );

    TESTLABS_CHECK( json.get_type() == dl7::json::Json::Type::Unsigned );
    TESTLABS_CHECK( json.is_unsigned() );
    TESTLABS_CHECK( json.is_number() );
    TESTLABS_CHECK( json.is_primitive() );
    TESTLABS_CHECK_EQ( json.to_string(), u8"7" );
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



TESTLABS_CASE( u8"DataLabs:  json:  JsonWriter:  is_valid_unquoted_key" )
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
        TESTLABS_CHECK_EQ( dl7::json::JsonWriter::is_valid_unquoted_key( entry.key ), entry.expected );
    }
}

TESTLABS_CASE( u8"DataLabs:  json:  JsonWriter:  to_string" )
{
    struct Entry
    {
        dl7::json::Json json;
        dl7::json::JsonWriter::Format format;
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
        TESTLABS_CHECK_EQ( dl7::json::JsonWriter::to_string( entry.json ), entry.string );
    }
}



#endif // F77_TESTS_DL7_JSON_H
