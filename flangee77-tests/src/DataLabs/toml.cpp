
#include <TestLabs/TestSuite.h>

#include <DataLabs/toml/Toml.h>
#include <DataLabs/toml/Value.h>
#include <DataLabs/toml/TomlReader.h>
#include <DataLabs/toml/TomlWriter.h>

#include "../shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const dl7::toml::Toml& toml) { return dl7::toml::TomlWriter::to_string( toml ); }
}



TESTLABS_CASE( u8"DataLabs:  toml:  Value:  none" )
{
    dl7::toml::Value value;

    TESTLABS_CHECK( value.get_type() == dl7::toml::Value::Type::None );
    TESTLABS_CHECK( value.is_none() );
    TESTLABS_CHECK( value.is_primitive() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"" );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  string" )
{
    dl7::toml::Value value( u8"Hello World" );

    TESTLABS_CHECK( value.get_type() == dl7::toml::Value::Type::String );
    TESTLABS_CHECK( value.is_string() );
    TESTLABS_CHECK( value.is_primitive() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"\"Hello World\"" );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  integer" )
{
    dl7::toml::Value value( -7 );

    TESTLABS_CHECK( value.get_type() == dl7::toml::Value::Type::Integer );
    TESTLABS_CHECK( value.is_integer() );
    TESTLABS_CHECK( value.is_number() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"-7" );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  float" )
{
    dl7::toml::Value value( 7.0 );

    TESTLABS_CHECK( value.get_type() == dl7::toml::Value::Type::Float );
    TESTLABS_CHECK( value.is_float() );
    TESTLABS_CHECK( value.is_number() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"7.0" );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  true" )
{
    dl7::toml::Value value( true );

    TESTLABS_CHECK( value.get_type() == dl7::toml::Value::Type::Boolean );
    TESTLABS_CHECK( value.is_boolean() );
    TESTLABS_CHECK( value.is_true() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"true" );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  false" )
{
    dl7::toml::Value value( false );

    TESTLABS_CHECK( value.get_type() == dl7::toml::Value::Type::Boolean );
    TESTLABS_CHECK( value.is_boolean() );
    TESTLABS_CHECK( value.is_false() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"false" );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  array" )
{
    dl7::toml::Value value( dl7::toml::Value::Type::Array );

    TESTLABS_CHECK( value.get_type() == dl7::toml::Value::Type::Array );
    TESTLABS_CHECK( value.is_array() );
    TESTLABS_CHECK( !value.is_primitive() );
    TESTLABS_CHECK( value.is_structured() );
    TESTLABS_CHECK( value.is_empty() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"[]" );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  table" )
{
    dl7::toml::Value value( dl7::toml::Value::Type::Table );

    TESTLABS_CHECK( value.get_type() == dl7::toml::Value::Type::Table );
    TESTLABS_CHECK( value.is_table() );
    TESTLABS_CHECK( !value.is_primitive() );
    TESTLABS_CHECK( value.is_structured() );
    TESTLABS_CHECK( value.is_empty() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"{}" );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  array (with entries)" )
{
    dl7::toml::Value value( dl7::toml::array_t{ dl7::toml::Value( 1 ), dl7::toml::Value( 2 ), dl7::toml::Value( 3 ) } );

    TESTLABS_CHECK( value.get_type() == dl7::toml::Value::Type::Array );
    TESTLABS_CHECK( value.is_array() );
    TESTLABS_CHECK( !value.is_primitive() );
    TESTLABS_CHECK( value.is_structured() );
    TESTLABS_CHECK( !value.is_empty() );
    TESTLABS_CHECK_EQ( value.as_array().size(), 3 );
    TESTLABS_CHECK_EQ( value.at( 0 ).as_integer(), 1 );
    TESTLABS_CHECK_EQ( value.to_string(), u8"[1, 2, 3]" );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  table (with entries)" )
{
    dl7::toml::Value value( dl7::toml::table_t{ { u8"a", dl7::toml::Value( 1 ) }, { u8"b", dl7::toml::Value( u8"two" ) } } );

    TESTLABS_CHECK( value.get_type() == dl7::toml::Value::Type::Table );
    TESTLABS_CHECK( value.is_table() );
    TESTLABS_CHECK( !value.is_primitive() );
    TESTLABS_CHECK( value.is_structured() );
    TESTLABS_CHECK( !value.is_empty() );
    TESTLABS_CHECK_EQ( value.as_table().size(), 2 );
    TESTLABS_CHECK_EQ( value.at( u8"a" ).as_integer(), 1 );
    TESTLABS_CHECK_EQ( value.at( u8"b" ).as_string(), u8"two" );
    TESTLABS_CHECK_EQ( value.to_string(), u8"{a = 1, b = \"two\"}" );
}



TESTLABS_CASE( u8"DataLabs:  toml:  Value:  copy-assigning none" )
{
    dl7::toml::Value value( 42 );

    value = dl7::toml::Value();

    TESTLABS_CHECK( value.get_type() == dl7::toml::Value::Type::None );
    TESTLABS_CHECK( value.is_none() );
    TESTLABS_CHECK_EQ( value.to_string(), u8"" );
}



TESTLABS_CASE( u8"DataLabs:  toml:  Value:  reset type" )
{
    dl7::toml::Value value( u8"Hello World" );

    value.reset_type( dl7::toml::Value::Type::Table );
    TESTLABS_CHECK( value.is_table() );
    TESTLABS_CHECK( value.is_empty() );

    value.reset_type( dl7::toml::Value::Type::Integer );
    TESTLABS_CHECK( value.is_integer() );
    TESTLABS_CHECK_EQ( value.as_integer(), 0 );

    value.reset_type( dl7::toml::Value::Type::None );
    TESTLABS_CHECK( value.is_none() );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  implicit collections via operator[]" )
{
    dl7::toml::Value value;

    value[ u8"servers" ][ u8"alpha" ] = dl7::toml::Value{ u8"10.0.0.1" };

    TESTLABS_CHECK( value.is_table() );
    TESTLABS_CHECK( value.at( u8"servers" ).is_table() );
    TESTLABS_CHECK_EQ( value.at( u8"servers" ).at( u8"alpha" ).as_string(), u8"10.0.0.1" );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  copy, move, swap, and comparison" )
{
    dl7::toml::Value value( dl7::toml::table_t{
        { u8"a", dl7::toml::Value( 1 ) },
        { u8"b", dl7::toml::Value( dl7::toml::array_t{ dl7::toml::Value( true ), dl7::toml::Value( 2 ) } ) },
    } );

    dl7::toml::Value copy{ value }; // NOLINT(performance-unnecessary-copy-initialization)
    TESTLABS_CHECK( copy == value );
    TESTLABS_CHECK( !(copy != value) );

    // The copy must be deep, i.e., modifying it must leave the original untouched.
    copy.at( u8"a" ).set_integer( 2 );
    TESTLABS_CHECK( copy != value );
    TESTLABS_CHECK_EQ( value.at( u8"a" ).as_integer(), 1 );

    dl7::toml::Value moved{ std::move( copy ) };
    TESTLABS_CHECK( moved.is_table() );
    TESTLABS_CHECK_EQ( moved.at( u8"a" ).as_integer(), 2 );

    dl7::toml::Value scalar( u8"x" );
    moved.swap( scalar );
    TESTLABS_CHECK( moved.is_string() );
    TESTLABS_CHECK( scalar.is_table() );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  as_number" )
{
    dl7::toml::Value integer( -7 );
    dl7::toml::Value decimal( 3.5 );

    TESTLABS_CHECK_EQ( integer.as_number<double>(), -7.0 );
    TESTLABS_CHECK_EQ( integer.as_number<int>(), -7 );
    TESTLABS_CHECK_EQ( decimal.as_number<double>(), 3.5 );
    TESTLABS_CHECK_EQ( decimal.as_number<int>(), 3 );
}

TESTLABS_CASE( u8"DataLabs:  toml:  Value:  is_true / is_false on non-booleans" )
{
    dl7::toml::Value none;
    dl7::toml::Value zero( 0 );
    dl7::toml::Value empty_string( u8"" );

    TESTLABS_CHECK( !none.is_true() );
    TESTLABS_CHECK( !none.is_false() );
    TESTLABS_CHECK( !zero.is_true() );
    TESTLABS_CHECK( !zero.is_false() );
    TESTLABS_CHECK( !empty_string.is_true() );
    TESTLABS_CHECK( !empty_string.is_false() );
}



TESTLABS_CASE( u8"DataLabs:  toml:  TomlReader:  parse" )
{
    using dl7::toml::Toml;
    using dl7::toml::Value;
    using dl7::toml::table_t;
    using dl7::toml::array_t;

    struct Entry
    {
        cl7::u8string string;
        Toml toml;
    } entry;

    const std::vector<Entry> container {
        { u8"", Toml() },
        { u8"foo = \"bar\"", Toml( table_t{ { u8"foo", Value( u8"bar" ) } } ) },
        { u8"foo = 'bar'", Toml( table_t{ { u8"foo", Value( u8"bar" ) } } ) },
        { u8"foo = 7", Toml( table_t{ { u8"foo", Value( 7 ) } } ) },
        { u8"foo = -7", Toml( table_t{ { u8"foo", Value( -7 ) } } ) },
        { u8"foo = +7", Toml( table_t{ { u8"foo", Value( 7 ) } } ) },
        { u8"foo = 7.5", Toml( table_t{ { u8"foo", Value( 7.5 ) } } ) },
        { u8"foo = true", Toml( table_t{ { u8"foo", Value( true ) } } ) },
        { u8"foo = false", Toml( table_t{ { u8"foo", Value( false ) } } ) },
        { u8"foo = \"7\"", Toml( table_t{ { u8"foo", Value( u8"7" ) } } ) },
        { u8"foo = \"true\"", Toml( table_t{ { u8"foo", Value( u8"true" ) } } ) },
        { u8"bare_key = 1", Toml( table_t{ { u8"bare_key", Value( 1 ) } } ) },
        { u8"\"quoted key\" = 1", Toml( table_t{ { u8"quoted key", Value( 1 ) } } ) },
        { u8"foo = [1, 2, 3]", Toml( table_t{ { u8"foo", Value( array_t{ Value( 1 ), Value( 2 ), Value( 3 ) } ) } } ) },
        { u8"foo = [ 1, 2, 3 ]", Toml( table_t{ { u8"foo", Value( array_t{ Value( 1 ), Value( 2 ), Value( 3 ) } ) } } ) },
        { u8"foo = [\n  1,\n  2,\n  3,\n]", Toml( table_t{ { u8"foo", Value( array_t{ Value( 1 ), Value( 2 ), Value( 3 ) } ) } } ) },
        { u8"foo = [\"a\", \"b\"]", Toml( table_t{ { u8"foo", Value( array_t{ Value( u8"a" ), Value( u8"b" ) } ) } } ) },
        { u8"# just a comment", Toml() },
        { u8"foo = 1 # trailing comment", Toml( table_t{ { u8"foo", Value( 1 ) } } ) },
        { u8"a = 1\nb = 2", Toml( table_t{ { u8"a", Value( 1 ) }, { u8"b", Value( 2 ) } } ) },
        { u8"[server]\nhost = \"localhost\"", Toml( table_t{ { u8"server", Value( table_t{ { u8"host", Value( u8"localhost" ) } } ) } } ) },
        { u8"[a.b]\nc = 1", Toml( table_t{ { u8"a", Value( table_t{ { u8"b", Value( table_t{ { u8"c", Value( 1 ) } } ) } } ) } } ) },
        { u8"a.b.c = 1", Toml( table_t{ { u8"a", Value( table_t{ { u8"b", Value( table_t{ { u8"c", Value( 1 ) } } ) } } ) } } ) },

        // Number robustness: underscores and base prefixes.
        { u8"foo = 1_000", Toml( table_t{ { u8"foo", Value( 1000 ) } } ) },
        { u8"foo = 1_000_000", Toml( table_t{ { u8"foo", Value( 1000000 ) } } ) },
        { u8"foo = 9_224.5", Toml( table_t{ { u8"foo", Value( 9224.5 ) } } ) },
        { u8"foo = 0xff", Toml( table_t{ { u8"foo", Value( 255 ) } } ) },
        { u8"foo = 0xDEAD_BEEF", Toml( table_t{ { u8"foo", Value( 0xDEADBEEF ) } } ) },
        { u8"foo = 0o17", Toml( table_t{ { u8"foo", Value( 15 ) } } ) },
        { u8"foo = 0b1010", Toml( table_t{ { u8"foo", Value( 10 ) } } ) },

        // Multi-line strings.
        { u8"foo = \"\"\"hello\"\"\"", Toml( table_t{ { u8"foo", Value( u8"hello" ) } } ) },
        { u8"foo = \"\"\"\nhello\"\"\"", Toml( table_t{ { u8"foo", Value( u8"hello" ) } } ) },
        { u8"foo = \"\"\"line1\nline2\"\"\"", Toml( table_t{ { u8"foo", Value( u8"line1\nline2" ) } } ) },
        { u8"foo = '''hello'''", Toml( table_t{ { u8"foo", Value( u8"hello" ) } } ) },
        { u8"foo = '''C:\\path\\no\\escapes'''", Toml( table_t{ { u8"foo", Value( u8"C:\\path\\no\\escapes" ) } } ) },
        { u8"foo = \"\"\"a \\\n  b\"\"\"", Toml( table_t{ { u8"foo", Value( u8"a b" ) } } ) },

        // Inline tables.
        { u8"foo = {}", Toml( table_t{ { u8"foo", Value( table_t{} ) } } ) },
        { u8"foo = { a = 1, b = 2 }", Toml( table_t{ { u8"foo", Value( table_t{ { u8"a", Value( 1 ) }, { u8"b", Value( 2 ) } } ) } } ) },
        { u8"foo = { a.b = 1 }", Toml( table_t{ { u8"foo", Value( table_t{ { u8"a", Value( table_t{ { u8"b", Value( 1 ) } } ) } } ) } } ) },

        // Array of tables.
        { u8"[[products]]\nname = \"A\"\n[[products]]\nname = \"B\"", Toml( table_t{ { u8"products", Value( array_t{ Value( table_t{ { u8"name", Value( u8"A" ) } } ), Value( table_t{ { u8"name", Value( u8"B" ) } } ) } ) } } ) },
        { u8"[[fruits]]\nname = \"apple\"\n[fruits.info]\ncolor = \"red\"", Toml( table_t{ { u8"fruits", Value( array_t{ Value( table_t{ { u8"name", Value( u8"apple" ) }, { u8"info", Value( table_t{ { u8"color", Value( u8"red" ) } } ) } } ) } ) } } ) },

        {
u8"# a small config\n"
"title = \"Example\"\n"
"\n"
"[owner]\n"
"name = \"Tom\"\n"
"\n"
"[database]\n"
"enabled = true\n"
"ports = [8000, 8001]\n"
"\r\n"
"crlf = 1\r\n",
            Toml( table_t{
                { u8"title", Value( u8"Example" ) },
                { u8"owner", Value( table_t{ { u8"name", Value( u8"Tom" ) } } ) },
                { u8"database", Value( table_t{
                    { u8"enabled", Value( true ) },
                    { u8"ports", Value( array_t{ Value( 8000 ), Value( 8001 ) } ) },
                    { u8"crlf", Value( 1 ) },
                } ) },
            } )
        },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.string )
    {
        const auto toml = dl7::toml::TomlReader::parse( entry.string );
        TESTLABS_CHECK_EQ( toml, entry.toml );
    }
}



TESTLABS_CASE( u8"DataLabs:  toml:  TomlWriter:  to_string" )
{
    using dl7::toml::Toml;
    using dl7::toml::Value;
    using dl7::toml::table_t;
    using dl7::toml::array_t;
    using dl7::toml::TomlWriter;

    struct Entry
    {
        Toml toml;
        dl7::toml::Format format;
        cl7::u8string string;
    } entry;

    dl7::toml::Format format;

    const std::vector<Entry> container {
        { Toml(), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"" },
        { Toml( table_t{ { u8"foo", Value( u8"bar" ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"foo = \"bar\"\n" },
        { Toml( table_t{ { u8"foo", Value( 7 ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"foo = 7\n" },
        { Toml( table_t{ { u8"foo", Value( -7 ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"foo = -7\n" },
        { Toml( table_t{ { u8"foo", Value( 7.0 ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"foo = 7.0\n" },
        { Toml( table_t{ { u8"foo", Value( true ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"foo = true\n" },
        { Toml( table_t{ { u8"foo", Value( false ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"foo = false\n" },
        { Toml( table_t{ { u8"foo", Value( array_t{ Value( 1 ), Value( 2 ), Value( 3 ) } ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"foo = [1, 2, 3]\n" },
        { Toml( table_t{ { u8"quoted key", Value( 1 ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"\"quoted key\" = 1\n" },
        { Toml( table_t{ { u8"server", Value( table_t{ { u8"host", Value( u8"localhost" ) } } ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"[server]\nhost = \"localhost\"\n" },
        { Toml( table_t{ { u8"a", Value( table_t{ { u8"b", Value( table_t{ { u8"c", Value( 1 ) } } ) } } ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"[a.b]\nc = 1\n" },
        { Toml( table_t{ { u8"products", Value( array_t{ Value( table_t{ { u8"name", Value( u8"A" ) } } ), Value( table_t{ { u8"name", Value( u8"B" ) } } ) } ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"[[products]]\nname = \"A\"\n[[products]]\nname = \"B\"\n" },
        { Toml( table_t{ { u8"fruits", Value( array_t{ Value( table_t{ { u8"name", Value( u8"apple" ) }, { u8"info", Value( table_t{ { u8"color", Value( u8"red" ) } } ) } } ) } ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"[[fruits]]\nname = \"apple\"\n[fruits.info]\ncolor = \"red\"\n" },
        { Toml( table_t{ { u8"nums", Value( array_t{ Value( 1 ), Value( 2 ) } ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"nums = [1, 2]\n" },
        { Toml( table_t{ { u8"empty", Value( array_t{} ) } } ), TomlWriter::DEFAULT_COMPACT_FORMAT, u8"empty = []\n" },
        { Toml( table_t{ { u8"foo", Value( u8"bar" ) } } ), (format = TomlWriter::DEFAULT_COMPACT_FORMAT, format.space_around_equals_sign = dl7::toml::Format::SpaceAroundEqualsSign::None, format), u8"foo=\"bar\"\n" },
        { Toml( table_t{ { u8"foo", Value( u8"bar" ) } } ), (format = TomlWriter::DEFAULT_COMPACT_FORMAT, format.line_ending = dl7::toml::Format::LineEnding::CRLF, format), u8"foo = \"bar\"\r\n" },
        { Toml( table_t{ { u8"foo", Value( u8"bar" ) } } ), TomlWriter::DEFAULT_PRETTY_PRINT_FORMAT, u8"\nfoo = \"bar\"\n\n" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.string )
    {
        const auto string = dl7::toml::TomlWriter::to_string( entry.toml, entry.format );
        TESTLABS_CHECK_EQ( string, entry.string );
    }
}



TESTLABS_CASE( u8"DataLabs:  toml:  date-time parsing" )
{
    using dl7::toml::Value;

    struct Entry
    {
        cl7::u8string input;
        Value::Type type;
        cl7::u8string canonical;
    } entry;

    const std::vector<Entry> container {
        { u8"2020-06-15", Value::Type::LocalDate, u8"2020-06-15" },
        { u8"12:30:45", Value::Type::LocalTime, u8"12:30:45" },
        { u8"2020-06-15T12:30:45", Value::Type::LocalDateTime, u8"2020-06-15T12:30:45" },
        { u8"2020-06-15 12:30:45", Value::Type::LocalDateTime, u8"2020-06-15T12:30:45" },
        { u8"2020-06-15T12:30:45Z", Value::Type::OffsetDateTime, u8"2020-06-15T12:30:45Z" },
        { u8"2020-06-15t12:30:45z", Value::Type::OffsetDateTime, u8"2020-06-15T12:30:45Z" },
        { u8"2020-06-15T12:30:45+02:00", Value::Type::OffsetDateTime, u8"2020-06-15T12:30:45+02:00" },
        { u8"2020-06-15T12:30:45-05:30", Value::Type::OffsetDateTime, u8"2020-06-15T12:30:45-05:30" },
        { u8"2020-06-15T12:30:45.500Z", Value::Type::OffsetDateTime, u8"2020-06-15T12:30:45Z" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.input )
    {
        cl7::u8string source = u8"d = ";
        source += entry.input;

        const auto toml = dl7::toml::TomlReader::parse( source );
        const auto& value = toml.at( u8"d" );

        TESTLABS_CHECK( value.get_type() == entry.type );
        TESTLABS_CHECK_EQ( value.to_string(), entry.canonical );

        // Round-trip: writing and re-parsing must yield an equal document.
        const auto reparsed = dl7::toml::TomlReader::parse( dl7::toml::TomlWriter::to_string( toml ) );
        TESTLABS_CHECK_EQ( reparsed, toml );
    }
}



TESTLABS_CASE( u8"DataLabs:  toml:  round trip (parse after write)" )
{
    using dl7::toml::Toml;
    using dl7::toml::Value;
    using dl7::toml::table_t;
    using dl7::toml::array_t;

    struct Entry
    {
        cl7::u8string label;
        Toml toml;
    } entry;

    const std::vector<Entry> container {
        { u8"empty", Toml() },
        { u8"string", Toml( table_t{ { u8"foo", Value( u8"bar baz" ) } } ) },
        { u8"integer", Toml( table_t{ { u8"foo", Value( -42 ) } } ) },
        { u8"float", Toml( table_t{ { u8"foo", Value( 3.5 ) } } ) },
        { u8"boolean", Toml( table_t{ { u8"foo", Value( true ) } } ) },
        { u8"array", Toml( table_t{ { u8"foo", Value( array_t{ Value( 1 ), Value( 2 ), Value( 3 ) } ) } } ) },
        { u8"nested tables", Toml( table_t{ { u8"a", Value( table_t{ { u8"b", Value( table_t{ { u8"c", Value( 1 ) } } ) } } ) } } ) },
        { u8"inline table value", Toml( table_t{ { u8"point", Value( table_t{ { u8"x", Value( 1 ) }, { u8"y", Value( 2 ) } } ) } } ) },
        { u8"array of tables", Toml( table_t{ { u8"items", Value( array_t{ Value( table_t{ { u8"n", Value( 1 ) } } ), Value( table_t{ { u8"n", Value( 2 ) } } ) } ) } } ) },
        { u8"multi-line-ish string", Toml( table_t{ { u8"text", Value( u8"line1\nline2" ) } } ) },
        {
            u8"mixed",
            Toml( table_t{
                { u8"title", Value( u8"Example" ) },
                { u8"database", Value( table_t{
                    { u8"enabled", Value( true ) },
                    { u8"ports", Value( array_t{ Value( 8000 ), Value( 8001 ) } ) },
                } ) },
            } )
        },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.label )
    {
        const auto string = dl7::toml::TomlWriter::to_string( entry.toml );
        const auto toml = dl7::toml::TomlReader::parse( string );
        TESTLABS_CHECK_EQ( toml, entry.toml );
    }
}
