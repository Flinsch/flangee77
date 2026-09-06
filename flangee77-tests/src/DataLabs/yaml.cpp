
#include <TestLabs/TestSuite.h>

#include <DataLabs/yaml/Yaml.h>
#include <DataLabs/yaml/YamlReader.h>
#include <DataLabs/yaml/YamlWriter.h>

#include "../shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const dl7::yaml::Yaml& yaml) { return yaml.to_string(); }
}



TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  null" )
{
    dl7::yaml::Yaml yaml;

    TESTLABS_CHECK( yaml.get_type() == dl7::yaml::Yaml::Type::Null );
    TESTLABS_CHECK( yaml.is_null() );
    TESTLABS_CHECK( yaml.is_primitive() );
    TESTLABS_CHECK( !yaml.is_structured() );
    TESTLABS_CHECK( yaml.is_empty() );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  mapping" )
{
    dl7::yaml::Yaml yaml( dl7::yaml::Yaml::Type::Mapping );

    TESTLABS_CHECK( yaml.get_type() == dl7::yaml::Yaml::Type::Mapping );
    TESTLABS_CHECK( yaml.is_mapping() );
    TESTLABS_CHECK( !yaml.is_primitive() );
    TESTLABS_CHECK( yaml.is_structured() );
    TESTLABS_CHECK( yaml.is_empty() );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  sequence" )
{
    dl7::yaml::Yaml yaml( dl7::yaml::Yaml::Type::Sequence );

    TESTLABS_CHECK( yaml.get_type() == dl7::yaml::Yaml::Type::Sequence );
    TESTLABS_CHECK( yaml.is_sequence() );
    TESTLABS_CHECK( !yaml.is_primitive() );
    TESTLABS_CHECK( yaml.is_structured() );
    TESTLABS_CHECK( yaml.is_empty() );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  string" )
{
    dl7::yaml::Yaml yaml( u8"Hello World" );

    TESTLABS_CHECK( yaml.get_type() == dl7::yaml::Yaml::Type::String );
    TESTLABS_CHECK( yaml.is_string() );
    TESTLABS_CHECK( yaml.is_primitive() );
    TESTLABS_CHECK( !yaml.is_structured() );
    TESTLABS_CHECK( !yaml.is_empty() );
    TESTLABS_CHECK_EQ( yaml.as_string(), u8"Hello World" );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  integer" )
{
    dl7::yaml::Yaml yaml( -7 );

    TESTLABS_CHECK( yaml.get_type() == dl7::yaml::Yaml::Type::Integer );
    TESTLABS_CHECK( yaml.is_integer() );
    TESTLABS_CHECK( yaml.is_number() );
    TESTLABS_CHECK( yaml.is_primitive() );
    TESTLABS_CHECK( !yaml.is_structured() );
    TESTLABS_CHECK( !yaml.is_empty() );
    TESTLABS_CHECK_EQ( yaml.as_integer(), -7 );
    TESTLABS_CHECK_EQ( yaml.as_number<double>(), -7.0 );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  float" )
{
    dl7::yaml::Yaml yaml( 7.0 );

    TESTLABS_CHECK( yaml.get_type() == dl7::yaml::Yaml::Type::Float );
    TESTLABS_CHECK( yaml.is_float() );
    TESTLABS_CHECK( yaml.is_number() );
    TESTLABS_CHECK( yaml.is_primitive() );
    TESTLABS_CHECK( !yaml.is_structured() );
    TESTLABS_CHECK( !yaml.is_empty() );
    TESTLABS_CHECK_EQ( yaml.as_float(), 7.0 );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  true" )
{
    dl7::yaml::Yaml yaml( true );

    TESTLABS_CHECK( yaml.get_type() == dl7::yaml::Yaml::Type::Boolean );
    TESTLABS_CHECK( yaml.is_boolean() );
    TESTLABS_CHECK( yaml.is_true() );
    TESTLABS_CHECK( !yaml.is_false() );
    TESTLABS_CHECK( yaml.is_primitive() );
    TESTLABS_CHECK( !yaml.is_structured() );
    TESTLABS_CHECK( !yaml.is_empty() );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  false" )
{
    dl7::yaml::Yaml yaml( false );

    TESTLABS_CHECK( yaml.get_type() == dl7::yaml::Yaml::Type::Boolean );
    TESTLABS_CHECK( yaml.is_boolean() );
    TESTLABS_CHECK( !yaml.is_true() );
    TESTLABS_CHECK( yaml.is_false() );
    TESTLABS_CHECK( yaml.is_primitive() );
    TESTLABS_CHECK( !yaml.is_structured() );
    TESTLABS_CHECK( !yaml.is_empty() );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  sequence (with entries)" )
{
    dl7::yaml::Yaml yaml( dl7::yaml::sequence_t{ dl7::yaml::Yaml{ 1 }, dl7::yaml::Yaml{ u8"two" }, dl7::yaml::Yaml{} } );

    TESTLABS_CHECK( yaml.get_type() == dl7::yaml::Yaml::Type::Sequence );
    TESTLABS_CHECK( yaml.is_sequence() );
    TESTLABS_CHECK( yaml.is_structured() );
    TESTLABS_CHECK( !yaml.is_primitive() );
    TESTLABS_CHECK( !yaml.is_empty() );
    TESTLABS_CHECK_EQ( yaml.as_sequence().size(), 3 );
    TESTLABS_CHECK_EQ( yaml.at( 0 ).as_integer(), 1 );
    TESTLABS_CHECK_EQ( yaml.at( 1 ).as_string(), u8"two" );
    TESTLABS_CHECK( yaml.at( 2 ).is_null() );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  mapping (with entries)" )
{
    dl7::yaml::Yaml yaml( dl7::yaml::mapping_t{ { u8"a", dl7::yaml::Yaml{ 1 } }, { u8"b", dl7::yaml::Yaml{ u8"two" } } } );

    TESTLABS_CHECK( yaml.get_type() == dl7::yaml::Yaml::Type::Mapping );
    TESTLABS_CHECK( yaml.is_mapping() );
    TESTLABS_CHECK( yaml.is_structured() );
    TESTLABS_CHECK( !yaml.is_primitive() );
    TESTLABS_CHECK( !yaml.is_empty() );
    TESTLABS_CHECK_EQ( yaml.as_mapping().size(), 2 );
    TESTLABS_CHECK_EQ( yaml.at( u8"a" ).as_integer(), 1 );
    TESTLABS_CHECK_EQ( yaml.at( u8"b" ).as_string(), u8"two" );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  reset type" )
{
    dl7::yaml::Yaml yaml( u8"Hello World" );

    yaml.reset_type( dl7::yaml::Yaml::Type::Mapping );
    TESTLABS_CHECK( yaml.is_mapping() );
    TESTLABS_CHECK( yaml.is_empty() );

    yaml.reset_type( dl7::yaml::Yaml::Type::Integer );
    TESTLABS_CHECK( yaml.is_integer() );
    TESTLABS_CHECK_EQ( yaml.as_integer(), 0 );

    yaml.reset_type( dl7::yaml::Yaml::Type::Null );
    TESTLABS_CHECK( yaml.is_null() );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  implicit collections via operator[]" )
{
    dl7::yaml::Yaml yaml;

    yaml[ u8"servers" ][ u8"alpha" ] = dl7::yaml::Yaml{ u8"10.0.0.1" };

    TESTLABS_CHECK( yaml.is_mapping() );
    TESTLABS_CHECK( yaml.at( u8"servers" ).is_mapping() );
    TESTLABS_CHECK_EQ( yaml.at( u8"servers" ).at( u8"alpha" ).as_string(), u8"10.0.0.1" );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  copy, move, swap, and comparison" )
{
    dl7::yaml::Yaml yaml( dl7::yaml::mapping_t{
        { u8"a", dl7::yaml::Yaml{ 1 } },
        { u8"b", dl7::yaml::Yaml{ dl7::yaml::sequence_t{ dl7::yaml::Yaml{ true }, dl7::yaml::Yaml{} } } },
    } );

    dl7::yaml::Yaml copy{ yaml }; // NOLINT(performance-unnecessary-copy-initialization)
    TESTLABS_CHECK( copy == yaml );
    TESTLABS_CHECK( !(copy != yaml) );

    copy.at( u8"a" ).set_integer( 2 );
    TESTLABS_CHECK( copy != yaml );
    TESTLABS_CHECK_EQ( yaml.at( u8"a" ).as_integer(), 1 );

    dl7::yaml::Yaml moved{ std::move( copy ) };
    TESTLABS_CHECK( moved.is_mapping() );
    TESTLABS_CHECK_EQ( moved.at( u8"a" ).as_integer(), 2 );

    dl7::yaml::Yaml scalar( u8"x" );
    moved.swap( scalar );
    TESTLABS_CHECK( moved.is_string() );
    TESTLABS_CHECK( scalar.is_mapping() );

    // Assigning a null node must actually reset the target.
    scalar = dl7::yaml::Yaml{};
    TESTLABS_CHECK( scalar.is_null() );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  as_number" )
{
    dl7::yaml::Yaml integer( -7 );
    dl7::yaml::Yaml decimal( 3.5 );

    TESTLABS_CHECK_EQ( integer.as_number<double>(), -7.0 );
    TESTLABS_CHECK_EQ( integer.as_number<int>(), -7 );
    TESTLABS_CHECK_EQ( decimal.as_number<double>(), 3.5 );
    TESTLABS_CHECK_EQ( decimal.as_number<int>(), 3 );
}

TESTLABS_CASE( u8"DataLabs:  yaml:  Yaml:  is_true / is_false on non-booleans" )
{
    dl7::yaml::Yaml null;
    dl7::yaml::Yaml zero( 0 );
    dl7::yaml::Yaml empty_string( u8"" );

    TESTLABS_CHECK( !null.is_true() );
    TESTLABS_CHECK( !null.is_false() );
    TESTLABS_CHECK( !zero.is_true() );
    TESTLABS_CHECK( !zero.is_false() );
    TESTLABS_CHECK( !empty_string.is_true() );
    TESTLABS_CHECK( !empty_string.is_false() );
}
