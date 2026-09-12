#include <TestLabs/TestSuite.h>

#include <DataLabs/config/Value.h>

#include "../shared.h"

#include <CoreLabs/text/format.h>

#include <vector>



namespace {

    /**
     * Renders a value for a failed check to print. The neutral model deliberately
     * has no serialization of its own, and until the format converters exist there
     * is nothing to borrow one from.
     */
    cl7::u8string dump(const dl7::config::Value& value)
    {
        using dl7::config::Value;

        switch ( value.get_type() )
        {
        case Value::Type::Null:
            return u8"null";
        case Value::Type::Boolean:
            return value.is_true() ? u8"true" : u8"false";
        case Value::Type::Integer:
            return cl7::text::format::to_string<cl7::u8string>( value.as_integer() );
        case Value::Type::Float:
            return cl7::text::format::to_string<cl7::u8string>( value.as_float(), 1 );
        case Value::Type::String:
            return u8"\"" + value.as_string() + u8"\"";
        case Value::Type::Sequence:
        {
            cl7::u8string result{ u8"[" };
            for ( size_t i = 0; i < value.as_sequence().size(); ++i )
                result += ( i > 0 ? u8", " : u8"" ) + dump( value.at( i ) );
            return result + u8"]";
        }
        case Value::Type::Mapping:
        {
            cl7::u8string result{ u8"{" };
            bool first = true;
            for ( const auto& p : value.as_mapping() )
            {
                if ( !first )
                    result += u8", ";
                first = false;
                result += p.first + u8": " + dump( p.second );
            }
            return result + u8"}";
        }
        }

        return {};
    }

} // anonymous namespace



namespace tl7::internals {
    inline
    cl7::u8string to_string(const dl7::config::Value& value) { return dump( value ); }
}



TESTLABS_CASE( u8"DataLabs:  config:  Value:  null" )
{
    dl7::config::Value value;

    TESTLABS_CHECK( value.get_type() == dl7::config::Value::Type::Null );
    TESTLABS_CHECK( value.is_null() );
    TESTLABS_CHECK( value.is_primitive() );
    TESTLABS_CHECK( !value.is_structured() );
    TESTLABS_CHECK( value.is_empty() );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  mapping" )
{
    dl7::config::Value value( dl7::config::Value::Type::Mapping );

    TESTLABS_CHECK( value.get_type() == dl7::config::Value::Type::Mapping );
    TESTLABS_CHECK( value.is_mapping() );
    TESTLABS_CHECK( !value.is_primitive() );
    TESTLABS_CHECK( value.is_structured() );
    TESTLABS_CHECK( value.is_empty() );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  sequence" )
{
    dl7::config::Value value( dl7::config::Value::Type::Sequence );

    TESTLABS_CHECK( value.get_type() == dl7::config::Value::Type::Sequence );
    TESTLABS_CHECK( value.is_sequence() );
    TESTLABS_CHECK( !value.is_primitive() );
    TESTLABS_CHECK( value.is_structured() );
    TESTLABS_CHECK( value.is_empty() );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  string" )
{
    dl7::config::Value value( u8"Hello World" );

    TESTLABS_CHECK( value.get_type() == dl7::config::Value::Type::String );
    TESTLABS_CHECK( value.is_string() );
    TESTLABS_CHECK( value.is_primitive() );
    TESTLABS_CHECK( !value.is_structured() );
    TESTLABS_CHECK( !value.is_empty() );
    TESTLABS_CHECK_EQ( value.as_string(), u8"Hello World" );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  integer" )
{
    dl7::config::Value value( -7 );

    TESTLABS_CHECK( value.get_type() == dl7::config::Value::Type::Integer );
    TESTLABS_CHECK( value.is_integer() );
    TESTLABS_CHECK( value.is_number() );
    TESTLABS_CHECK( value.is_primitive() );
    TESTLABS_CHECK( !value.is_structured() );
    TESTLABS_CHECK( !value.is_empty() );
    TESTLABS_CHECK_EQ( value.as_integer(), -7 );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  float" )
{
    dl7::config::Value value( 7.0 );

    TESTLABS_CHECK( value.get_type() == dl7::config::Value::Type::Float );
    TESTLABS_CHECK( value.is_float() );
    TESTLABS_CHECK( value.is_number() );
    TESTLABS_CHECK( value.is_primitive() );
    TESTLABS_CHECK( !value.is_structured() );
    TESTLABS_CHECK( !value.is_empty() );
    TESTLABS_CHECK_EQ( value.as_float(), 7.0 );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  true" )
{
    dl7::config::Value value( true );

    TESTLABS_CHECK( value.get_type() == dl7::config::Value::Type::Boolean );
    TESTLABS_CHECK( value.is_boolean() );
    TESTLABS_CHECK( value.is_true() );
    TESTLABS_CHECK( !value.is_false() );
    TESTLABS_CHECK( value.is_primitive() );
    TESTLABS_CHECK( !value.is_empty() );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  false" )
{
    dl7::config::Value value( false );

    TESTLABS_CHECK( value.get_type() == dl7::config::Value::Type::Boolean );
    TESTLABS_CHECK( value.is_boolean() );
    TESTLABS_CHECK( !value.is_true() );
    TESTLABS_CHECK( value.is_false() );
    TESTLABS_CHECK( value.is_primitive() );
    TESTLABS_CHECK( !value.is_empty() );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  sequence (with entries)" )
{
    dl7::config::Value value( dl7::config::sequence_t{ dl7::config::Value{ 1 }, dl7::config::Value{ u8"two" }, dl7::config::Value{} } );

    TESTLABS_CHECK( value.is_sequence() );
    TESTLABS_CHECK( value.is_structured() );
    TESTLABS_CHECK( !value.is_empty() );
    TESTLABS_CHECK_EQ( value.as_sequence().size(), 3 );
    TESTLABS_CHECK_EQ( value.at( 0 ).as_integer(), 1 );
    TESTLABS_CHECK_EQ( value.at( 1 ).as_string(), u8"two" );
    TESTLABS_CHECK( value.at( 2 ).is_null() );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  mapping (with entries)" )
{
    dl7::config::Value value( dl7::config::mapping_t{ { u8"a", dl7::config::Value{ 1 } }, { u8"b", dl7::config::Value{ u8"two" } } } );

    TESTLABS_CHECK( value.is_mapping() );
    TESTLABS_CHECK( value.is_structured() );
    TESTLABS_CHECK( !value.is_empty() );
    TESTLABS_CHECK_EQ( value.as_mapping().size(), 2 );
    TESTLABS_CHECK_EQ( value.at( u8"a" ).as_integer(), 1 );
    TESTLABS_CHECK_EQ( value.at( u8"b" ).as_string(), u8"two" );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  reset type" )
{
    dl7::config::Value value( u8"Hello World" );

    value.reset_type( dl7::config::Value::Type::Mapping );
    TESTLABS_CHECK( value.is_mapping() );
    TESTLABS_CHECK( value.is_empty() );

    value.reset_type( dl7::config::Value::Type::Integer );
    TESTLABS_CHECK( value.is_integer() );
    TESTLABS_CHECK_EQ( value.as_integer(), 0 );

    value.reset_type( dl7::config::Value::Type::Null );
    TESTLABS_CHECK( value.is_null() );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  implicit collections via operator[]" )
{
    dl7::config::Value value;

    value[ u8"video" ][ u8"driver_name" ] = dl7::config::Value{ u8"direct3d11" };

    TESTLABS_CHECK( value.is_mapping() );
    TESTLABS_CHECK( value.at( u8"video" ).is_mapping() );
    TESTLABS_CHECK_EQ( value.at( u8"video" ).at( u8"driver_name" ).as_string(), u8"direct3d11" );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  copy, move, swap, and comparison" )
{
    dl7::config::Value value( dl7::config::mapping_t{
        { u8"a", dl7::config::Value{ 1 } },
        { u8"b", dl7::config::Value{ dl7::config::sequence_t{ dl7::config::Value{ true }, dl7::config::Value{} } } },
    } );

    dl7::config::Value copy{ value }; // NOLINT(performance-unnecessary-copy-initialization)
    TESTLABS_CHECK( copy == value );
    TESTLABS_CHECK( !(copy != value) );

    // The copy must be deep, i.e., modifying it must leave the original untouched.
    copy.at( u8"a" ).set_integer( 2 );
    TESTLABS_CHECK( copy != value );
    TESTLABS_CHECK_EQ( value.at( u8"a" ).as_integer(), 1 );

    dl7::config::Value moved{ std::move( copy ) };
    TESTLABS_CHECK( moved.is_mapping() );
    TESTLABS_CHECK_EQ( moved.at( u8"a" ).as_integer(), 2 );

    dl7::config::Value scalar( u8"x" );
    moved.swap( scalar );
    TESTLABS_CHECK( moved.is_string() );
    TESTLABS_CHECK( scalar.is_mapping() );

    // Assigning a null value must actually reset the target.
    scalar = dl7::config::Value{};
    TESTLABS_CHECK( scalar.is_null() );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  as_number" )
{
    dl7::config::Value integer( -7 );
    dl7::config::Value decimal( 3.5 );

    TESTLABS_CHECK_EQ( integer.as_number<double>(), -7.0 );
    TESTLABS_CHECK_EQ( integer.as_number<int>(), -7 );
    TESTLABS_CHECK_EQ( decimal.as_number<double>(), 3.5 );
    TESTLABS_CHECK_EQ( decimal.as_number<int>(), 3 );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  is_true / is_false on non-booleans" )
{
    dl7::config::Value null;
    dl7::config::Value zero( 0 );
    dl7::config::Value empty_string( u8"" );

    TESTLABS_CHECK( !null.is_true() );
    TESTLABS_CHECK( !null.is_false() );
    TESTLABS_CHECK( !zero.is_true() );
    TESTLABS_CHECK( !zero.is_false() );
    TESTLABS_CHECK( !empty_string.is_true() );
    TESTLABS_CHECK( !empty_string.is_false() );
}

TESTLABS_CASE( u8"DataLabs:  config:  Value:  merge" )
{
    using dl7::config::Value;
    using dl7::config::mapping_t;
    using dl7::config::sequence_t;

    struct Entry
    {
        cl7::u8string label;
        Value base;
        Value overlay;
        Value expected;
    } entry;

    const std::vector<Entry> container {
        {
            u8"an overlay fills in what it has, the rest stays",
            Value( mapping_t{ { u8"a", Value( 1 ) }, { u8"b", Value( mapping_t{ { u8"c", Value( 1 ) }, { u8"d", Value( 2 ) } } ) } } ),
            Value( mapping_t{ { u8"b", Value( mapping_t{ { u8"c", Value( 9 ) } } ) } } ),
            Value( mapping_t{ { u8"a", Value( 1 ) }, { u8"b", Value( mapping_t{ { u8"c", Value( 9 ) }, { u8"d", Value( 2 ) } } ) } } ),
        },
        {
            u8"a key the base does not have is added",
            Value( mapping_t{ { u8"a", Value( 1 ) } } ),
            Value( mapping_t{ { u8"b", Value( 2 ) } } ),
            Value( mapping_t{ { u8"a", Value( 1 ) }, { u8"b", Value( 2 ) } } ),
        },
        {
            u8"an empty overlay changes nothing",
            Value( mapping_t{ { u8"a", Value( 1 ) } } ),
            Value( mapping_t{} ),
            Value( mapping_t{ { u8"a", Value( 1 ) } } ),
        },
        {
            u8"a sequence is replaced, not appended to",
            Value( mapping_t{ { u8"a", Value( sequence_t{ Value( 1 ), Value( 2 ) } ) } } ),
            Value( mapping_t{ { u8"a", Value( sequence_t{ Value( 3 ) } ) } } ),
            Value( mapping_t{ { u8"a", Value( sequence_t{ Value( 3 ) } ) } } ),
        },
        {
            u8"an explicit null counts as a value like any other",
            Value( mapping_t{ { u8"a", Value( 1 ) } } ),
            Value( mapping_t{ { u8"a", Value() } } ),
            Value( mapping_t{ { u8"a", Value() } } ),
        },
        {
            u8"the overlay's type wins",
            Value( mapping_t{ { u8"a", Value( 1 ) } } ),
            Value( mapping_t{ { u8"a", Value( u8"x" ) } } ),
            Value( mapping_t{ { u8"a", Value( u8"x" ) } } ),
        },
        {
            u8"a mapping over a scalar replaces it",
            Value( mapping_t{ { u8"a", Value( 1 ) } } ),
            Value( mapping_t{ { u8"a", Value( mapping_t{ { u8"b", Value( 1 ) } } ) } } ),
            Value( mapping_t{ { u8"a", Value( mapping_t{ { u8"b", Value( 1 ) } } ) } } ),
        },
        {
            u8"a scalar over a mapping replaces it",
            Value( mapping_t{ { u8"a", Value( mapping_t{ { u8"b", Value( 1 ) } } ) } } ),
            Value( mapping_t{ { u8"a", Value( 1 ) } } ),
            Value( mapping_t{ { u8"a", Value( 1 ) } } ),
        },
        {
            u8"merging onto anything but a mapping replaces it wholesale",
            Value( 1 ),
            Value( mapping_t{ { u8"a", Value( 1 ) } } ),
            Value( mapping_t{ { u8"a", Value( 1 ) } } ),
        },
        {
            u8"nesting is merged all the way down",
            Value( mapping_t{ { u8"a", Value( mapping_t{ { u8"b", Value( mapping_t{ { u8"c", Value( 1 ) }, { u8"d", Value( 2 ) } } ) } } ) } } ),
            Value( mapping_t{ { u8"a", Value( mapping_t{ { u8"b", Value( mapping_t{ { u8"d", Value( 9 ) } } ) } } ) } } ),
            Value( mapping_t{ { u8"a", Value( mapping_t{ { u8"b", Value( mapping_t{ { u8"c", Value( 1 ) }, { u8"d", Value( 9 ) } } ) } } ) } } ),
        },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.label )
    {
        const auto overlay_before = entry.overlay;

        auto merged = entry.base;
        merged.merge( entry.overlay );

        TESTLABS_CHECK_EQ( merged, entry.expected );

        // Merging must leave the overlay alone.
        TESTLABS_CHECK_EQ( entry.overlay, overlay_before );
    }
}
