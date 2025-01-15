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



#endif // F77_TESTS_DL7_JSON_H
