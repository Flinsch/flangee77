#include <TestLabs/TestSuite.h>

#include <DataLabs/config/Value.h>
#include <DataLabs/config/Binder.h>

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


// #############################################################################
// The binding layer, on a fixture covering every shape a field can take.
// #############################################################################

namespace binding_test {

    enum struct Mode
    {
        Windowed,
        Borderless,
        Exclusive,
    };

    struct Limits
    {
        unsigned memory = 512;
        cl7::u8string label = u8"default";

        bool operator==(const Limits&) const = default;
    };

    struct Settings
    {
        bool enabled = true;
        int offset = -7;
        unsigned width = 640;
        double factor = 1.5;
        cl7::u8string title = u8"untitled";
        Mode mode = Mode::Windowed;
        Limits limits;
        std::vector<int> ports{ 1, 2 };
        std::vector<Limits> tiers;

        bool operator==(const Settings&) const = default;
    };

} // namespace binding_test

namespace dl7::config {

    template <>
    struct EnumNames<binding_test::Mode>
    {
        static constexpr auto names = std::make_tuple(
            enum_value( u8"windowed", binding_test::Mode::Windowed ),
            enum_value( u8"borderless", binding_test::Mode::Borderless ),
            enum_value( u8"exclusive", binding_test::Mode::Exclusive ) );
    };

    template <>
    struct Binding<binding_test::Limits>
    {
        static constexpr auto fields = std::make_tuple(
            field( u8"memory", &binding_test::Limits::memory ),
            field( u8"label", &binding_test::Limits::label ) );
    };

    template <>
    struct Binding<binding_test::Settings>
    {
        static constexpr auto fields = std::make_tuple(
            field( u8"enabled", &binding_test::Settings::enabled ),
            field( u8"offset", &binding_test::Settings::offset ),
            field( u8"width", &binding_test::Settings::width ),
            field( u8"factor", &binding_test::Settings::factor ),
            field( u8"title", &binding_test::Settings::title ),
            field( u8"mode", &binding_test::Settings::mode ),
            field( u8"limits", &binding_test::Settings::limits ),
            field( u8"ports", &binding_test::Settings::ports ),
            field( u8"tiers", &binding_test::Settings::tiers ) );
    };

} // namespace dl7::config



TESTLABS_CASE( u8"DataLabs:  config:  Binder:  load (what is not stated keeps its default)" )
{
    using dl7::config::Value;
    using dl7::config::mapping_t;

    const binding_test::Settings defaults;

    // An empty mapping is a configuration that states nothing at all.
    {
        dl7::config::Binder binder;
        binding_test::Settings settings;
        binder.load( Value( mapping_t{} ), settings );

        TESTLABS_CHECK( settings == defaults );
        TESTLABS_CHECK_EQ( binder.get_diagnostics().get_count(), 0 );
    }

    // Stating one field leaves the others alone.
    {
        dl7::config::Binder binder;
        binding_test::Settings settings;
        binder.load( Value( mapping_t{ { u8"width", Value( 1920 ) } } ), settings );

        TESTLABS_CHECK_EQ( settings.width, 1920 );
        TESTLABS_CHECK_EQ( settings.title, defaults.title );
        TESTLABS_CHECK_EQ( settings.offset, defaults.offset );
        TESTLABS_CHECK( settings.ports == defaults.ports );
        TESTLABS_CHECK_EQ( binder.get_diagnostics().get_count(), 0 );
    }
}

TESTLABS_CASE( u8"DataLabs:  config:  Binder:  load (every shape of field)" )
{
    using dl7::config::Value;
    using dl7::config::mapping_t;
    using dl7::config::sequence_t;

    dl7::config::Binder binder;
    binding_test::Settings settings;

    binder.load( Value( mapping_t{
        { u8"enabled", Value( false ) },
        { u8"offset", Value( 42 ) },
        { u8"width", Value( 1920 ) },
        { u8"factor", Value( 2.5 ) },
        { u8"title", Value( u8"Example" ) },
        { u8"mode", Value( u8"exclusive" ) },
        { u8"limits", Value( mapping_t{ { u8"memory", Value( 1024 ) } } ) },
        { u8"ports", Value( sequence_t{ Value( 7 ), Value( 8 ), Value( 9 ) } ) },
        { u8"tiers", Value( sequence_t{ Value( mapping_t{ { u8"label", Value( u8"low" ) } } ) } ) },
    } ), settings );

    TESTLABS_CHECK_EQ( binder.get_diagnostics().get_count(), 0 );

    TESTLABS_CHECK( !settings.enabled );
    TESTLABS_CHECK_EQ( settings.offset, 42 );
    TESTLABS_CHECK_EQ( settings.width, 1920 );
    TESTLABS_CHECK_EQ( settings.factor, 2.5 );
    TESTLABS_CHECK_EQ( settings.title, u8"Example" );
    TESTLABS_CHECK( settings.mode == binding_test::Mode::Exclusive );

    // A nested structure is laid over its own defaults, too.
    TESTLABS_CHECK_EQ( settings.limits.memory, 1024 );
    TESTLABS_CHECK_EQ( settings.limits.label, u8"default" );

    // A sequence is replaced rather than added to.
    TESTLABS_CHECK_EQ( settings.ports.size(), 3 );
    TESTLABS_CHECK_EQ( settings.tiers.size(), 1 );
    if ( settings.tiers.size() == 1 )
    {
        TESTLABS_CHECK_EQ( settings.tiers[ 0 ].label, u8"low" );
        TESTLABS_CHECK_EQ( settings.tiers[ 0 ].memory, 512 );
    }
}

TESTLABS_CASE( u8"DataLabs:  config:  Binder:  load (a string where a format has no types)" )
{
    using dl7::config::Value;
    using dl7::config::mapping_t;

    dl7::config::Binder binder;
    binding_test::Settings settings;

    // INI and XML hand everything over as a string, so a string that reads as a
    // number or a boolean is accepted for a field of that type.
    binder.load( Value( mapping_t{
        { u8"enabled", Value( u8"false" ) },
        { u8"offset", Value( u8"-21" ) },
        { u8"factor", Value( u8"0.25" ) },
    } ), settings );

    TESTLABS_CHECK_EQ( binder.get_diagnostics().get_count(), 0 );
    TESTLABS_CHECK( !settings.enabled );
    TESTLABS_CHECK_EQ( settings.offset, -21 );
    TESTLABS_CHECK_EQ( settings.factor, 0.25 );
}

TESTLABS_CASE( u8"DataLabs:  config:  Binder:  load (what gets reported)" )
{
    using dl7::config::Value;
    using dl7::config::mapping_t;
    using dl7::config::sequence_t;

    struct Entry
    {
        cl7::u8string label;
        Value value;
        cl7::u8string expected_message;
        bool expected_error = true;
    } entry;

    const std::vector<Entry> container {
        {
            u8"a key no field answers to",
            Value( mapping_t{ { u8"widht", Value( 1920 ) } } ),
            u8"widht: Unknown key.",
            false,
        },
        {
            u8"a key no field answers to, nested",
            Value( mapping_t{ { u8"limits", Value( mapping_t{ { u8"memroy", Value( 1 ) } } ) } } ),
            u8"limits.memroy: Unknown key.",
            false,
        },
        {
            u8"a number where a string belongs",
            Value( mapping_t{ { u8"title", Value( 1 ) } } ),
            u8"title: A string is expected here.",
        },
        {
            u8"a string where a number belongs",
            Value( mapping_t{ { u8"width", Value( u8"wide" ) } } ),
            u8"width: A whole number is expected here.",
        },
        {
            u8"a number where a boolean belongs",
            Value( mapping_t{ { u8"enabled", Value( 1 ) } } ),
            u8"enabled: A boolean is expected here.",
        },
        {
            u8"a name no enumeration value goes by",
            Value( mapping_t{ { u8"mode", Value( u8"fullscreen" ) } } ),
            u8"mode: Unknown name.",
        },
        {
            u8"a number where an enumeration belongs",
            Value( mapping_t{ { u8"mode", Value( 2 ) } } ),
            u8"mode: A name is expected here.",
        },
        {
            u8"a scalar where a mapping belongs",
            Value( mapping_t{ { u8"limits", Value( 1 ) } } ),
            u8"limits: A mapping is expected here.",
        },
        {
            u8"a scalar where a sequence belongs",
            Value( mapping_t{ { u8"ports", Value( 1 ) } } ),
            u8"ports: A sequence is expected here.",
        },
        {
            u8"a bad entry, pointed at by its index",
            Value( mapping_t{ { u8"ports", Value( sequence_t{ Value( 1 ), Value( u8"two" ) } ) } } ),
            u8"ports[1]: A whole number is expected here.",
        },
        {
            u8"a number the field has no room for",
            Value( mapping_t{ { u8"width", Value( -1 ) } } ),
            u8"width: This number is out of the field's range.",
        },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.label )
    {
        dl7::config::Binder binder;
        binding_test::Settings settings;
        binder.load( entry.value, settings );

        const auto& diagnostics = binder.get_diagnostics();

        TESTLABS_CHECK_EQ( diagnostics.get_count(), 1 );
        if ( !diagnostics.get_all().empty() )
        {
            TESTLABS_CHECK_EQ( diagnostics.get_all().front().message, entry.expected_message );
            TESTLABS_CHECK_EQ( diagnostics.get_error_count(), entry.expected_error ? 1 : 0 );
        }
    }
}

TESTLABS_CASE( u8"DataLabs:  config:  Binder:  load (a field that fails costs only itself)" )
{
    using dl7::config::Value;
    using dl7::config::mapping_t;

    const binding_test::Settings defaults;

    dl7::config::Binder binder;
    binding_test::Settings settings;

    binder.load( Value( mapping_t{
        { u8"width", Value( u8"wide" ) },
        { u8"title", Value( u8"Example" ) },
        { u8"nonsense", Value( 1 ) },
    } ), settings );

    // The bad field keeps its default, the good one is read all the same.
    TESTLABS_CHECK_EQ( settings.width, defaults.width );
    TESTLABS_CHECK_EQ( settings.title, u8"Example" );
    TESTLABS_CHECK_EQ( binder.get_diagnostics().get_error_count(), 1 );
    TESTLABS_CHECK_EQ( binder.get_diagnostics().get_warning_count(), 1 );
}

TESTLABS_CASE( u8"DataLabs:  config:  Binder:  save" )
{
    using dl7::config::Value;
    using dl7::config::mapping_t;
    using dl7::config::sequence_t;

    binding_test::Settings settings;
    settings.enabled = false;
    settings.offset = 3;
    settings.width = 800;
    settings.factor = 0.5;
    settings.title = u8"Example";
    settings.mode = binding_test::Mode::Borderless;
    settings.limits = { 256, u8"small" };
    settings.ports = { 5 };
    settings.tiers = { { 1, u8"one" } };

    dl7::config::Binder binder;
    const auto value = binder.save( settings );

    TESTLABS_CHECK_EQ( binder.get_diagnostics().get_count(), 0 );

    TESTLABS_CHECK_EQ( value, Value( mapping_t{
        { u8"enabled", Value( false ) },
        { u8"offset", Value( 3 ) },
        { u8"width", Value( 800 ) },
        { u8"factor", Value( 0.5 ) },
        { u8"title", Value( u8"Example" ) },
        { u8"mode", Value( u8"borderless" ) },
        { u8"limits", Value( mapping_t{ { u8"memory", Value( 256 ) }, { u8"label", Value( u8"small" ) } } ) },
        { u8"ports", Value( sequence_t{ Value( 5 ) } ) },
        { u8"tiers", Value( sequence_t{ Value( mapping_t{ { u8"memory", Value( 1 ) }, { u8"label", Value( u8"one" ) } } ) } ) },
    } ) );
}

TESTLABS_CASE( u8"DataLabs:  config:  Binder:  round trip (load after save)" )
{
    binding_test::Settings source;
    source.enabled = false;
    source.offset = -1234;
    source.width = 2560;
    source.factor = 1.25;
    source.title = u8"Round Trip";
    source.mode = binding_test::Mode::Exclusive;
    source.limits = { 4096, u8"large" };
    source.ports = { 1, 2, 3 };
    source.tiers = { { 1, u8"a" }, { 2, u8"b" } };

    dl7::config::Binder binder;
    const auto value = binder.save( source );

    binding_test::Settings target;
    binder.load( value, target );

    TESTLABS_CHECK_EQ( binder.get_diagnostics().get_count(), 0 );
    TESTLABS_CHECK( target == source );
}

TESTLABS_CASE( u8"DataLabs:  config:  Binder:  save (an enumeration value with no name)" )
{
    binding_test::Settings settings;
    settings.mode = static_cast<binding_test::Mode>( 99 );

    dl7::config::Binder binder;
    const auto value = binder.save( settings );

    TESTLABS_CHECK( binder.get_diagnostics().get_error_count() > 0 );
    TESTLABS_CHECK( value.at( u8"mode" ).is_null() );
}
