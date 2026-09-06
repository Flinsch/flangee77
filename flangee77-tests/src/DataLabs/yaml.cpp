
#include <TestLabs/TestSuite.h>

#include <DataLabs/yaml/Yaml.h>
#include <DataLabs/yaml/YamlReader.h>
#include <DataLabs/yaml/YamlWriter.h>

#include "../shared.h"

#include <cmath>
#include <limits>
#include <vector>



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


TESTLABS_CASE( u8"DataLabs:  yaml:  YamlReader:  parse" )
{
    using dl7::yaml::Yaml;
    using dl7::yaml::mapping_t;
    using dl7::yaml::sequence_t;

    struct Entry
    {
        cl7::u8string string;
        Yaml yaml;
    } entry;

    const std::vector<Entry> container {
        // Empty and comment-only documents.
        { u8"", Yaml() },
        { u8"\n\n", Yaml() },
        { u8"# just a comment", Yaml() },
        { u8"# a comment\n\n# and another one\n", Yaml() },

        // A document may be a plain scalar all by itself.
        { u8"hello", Yaml( u8"hello" ) },
        { u8"42", Yaml( 42 ) },
        { u8"hello world", Yaml( u8"hello world" ) },

        // Block mappings.
        { u8"key: value", Yaml( mapping_t{ { u8"key", Yaml( u8"value" ) } } ) },
        { u8"key: 42", Yaml( mapping_t{ { u8"key", Yaml( 42 ) } } ) },
        { u8"key:", Yaml( mapping_t{ { u8"key", Yaml() } } ) },
        { u8"a: 1\nb: 2", Yaml( mapping_t{ { u8"a", Yaml( 1 ) }, { u8"b", Yaml( 2 ) } } ) },
        { u8"a: 1\n\nb: 2\n", Yaml( mapping_t{ { u8"a", Yaml( 1 ) }, { u8"b", Yaml( 2 ) } } ) },
        { u8"a: 1\r\nb: 2\r\n", Yaml( mapping_t{ { u8"a", Yaml( 1 ) }, { u8"b", Yaml( 2 ) } } ) },
        { u8"outer:\n  inner: 1", Yaml( mapping_t{ { u8"outer", Yaml( mapping_t{ { u8"inner", Yaml( 1 ) } } ) } } ) },
        { u8"a:\n  b:\n    c: 1", Yaml( mapping_t{ { u8"a", Yaml( mapping_t{ { u8"b", Yaml( mapping_t{ { u8"c", Yaml( 1 ) } } ) } } ) } } ) },
        { u8"a:\n  b: 1\nc: 2", Yaml( mapping_t{ { u8"a", Yaml( mapping_t{ { u8"b", Yaml( 1 ) } } ) }, { u8"c", Yaml( 2 ) } } ) },

        // Block sequences.
        { u8"- 1\n- 2\n- 3", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ), Yaml( 3 ) } ) },
        { u8"- a\n- b\n", Yaml( sequence_t{ Yaml( u8"a" ), Yaml( u8"b" ) } ) },
        { u8"-\n- 2", Yaml( sequence_t{ Yaml(), Yaml( 2 ) } ) },
        { u8"key:\n  - 1\n  - 2", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) } } ) },
        // A block sequence may share the indentation of its parent key.
        { u8"key:\n- 1\n- 2", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) } } ) },
        { u8"key:\n- 1\nother: 2", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( 1 ) } ) }, { u8"other", Yaml( 2 ) } } ) },
        // Nested sequences, in compact notation and otherwise.
        { u8"- - 1\n  - 2", Yaml( sequence_t{ Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) } ) },
        { u8"-\n  - 1\n-\n  - 2", Yaml( sequence_t{ Yaml( sequence_t{ Yaml( 1 ) } ), Yaml( sequence_t{ Yaml( 2 ) } ) } ) },

        // Mappings inside sequences (compact notation).
        { u8"- name: a\n  age: 1\n- name: b", Yaml( sequence_t{ Yaml( mapping_t{ { u8"name", Yaml( u8"a" ) }, { u8"age", Yaml( 1 ) } } ), Yaml( mapping_t{ { u8"name", Yaml( u8"b" ) } } ) } ) },
        { u8"items:\n  - id: 1\n  - id: 2", Yaml( mapping_t{ { u8"items", Yaml( sequence_t{ Yaml( mapping_t{ { u8"id", Yaml( 1 ) } } ), Yaml( mapping_t{ { u8"id", Yaml( 2 ) } } ) } ) } } ) },

        // Comments.
        { u8"key: value # trailing comment", Yaml( mapping_t{ { u8"key", Yaml( u8"value" ) } } ) },
        { u8"# leading\nkey: 1\n# trailing", Yaml( mapping_t{ { u8"key", Yaml( 1 ) } } ) },
        // A `#` only starts a comment where it follows whitespace.
        { u8"key: a#b", Yaml( mapping_t{ { u8"key", Yaml( u8"a#b" ) } } ) },
        { u8"key: \"quoted # not a comment\"", Yaml( mapping_t{ { u8"key", Yaml( u8"quoted # not a comment" ) } } ) },

        // Core schema type resolution.
        { u8"key: null", Yaml( mapping_t{ { u8"key", Yaml() } } ) },
        { u8"key: Null", Yaml( mapping_t{ { u8"key", Yaml() } } ) },
        { u8"key: ~", Yaml( mapping_t{ { u8"key", Yaml() } } ) },
        { u8"key: true", Yaml( mapping_t{ { u8"key", Yaml( true ) } } ) },
        { u8"key: True", Yaml( mapping_t{ { u8"key", Yaml( true ) } } ) },
        { u8"key: FALSE", Yaml( mapping_t{ { u8"key", Yaml( false ) } } ) },
        { u8"key: -7", Yaml( mapping_t{ { u8"key", Yaml( -7 ) } } ) },
        { u8"key: +7", Yaml( mapping_t{ { u8"key", Yaml( 7 ) } } ) },
        { u8"key: -0", Yaml( mapping_t{ { u8"key", Yaml( 0 ) } } ) },
        { u8"key: 0x1f", Yaml( mapping_t{ { u8"key", Yaml( 31 ) } } ) },
        { u8"key: 0o17", Yaml( mapping_t{ { u8"key", Yaml( 15 ) } } ) },
        { u8"key: 7.5", Yaml( mapping_t{ { u8"key", Yaml( 7.5 ) } } ) },
        { u8"key: -7.5e2", Yaml( mapping_t{ { u8"key", Yaml( -750.0 ) } } ) },
        { u8"key: .inf", Yaml( mapping_t{ { u8"key", Yaml( std::numeric_limits<double>::infinity() ) } } ) },
        { u8"key: -.INF", Yaml( mapping_t{ { u8"key", Yaml( -std::numeric_limits<double>::infinity() ) } } ) },
        // Anything the core schema does not cover is a string, including the
        // booleans YAML 1.1 used to know and the number spellings it does not.
        { u8"key: yes", Yaml( mapping_t{ { u8"key", Yaml( u8"yes" ) } } ) },
        { u8"key: off", Yaml( mapping_t{ { u8"key", Yaml( u8"off" ) } } ) },
        { u8"key: inf", Yaml( mapping_t{ { u8"key", Yaml( u8"inf" ) } } ) },
        { u8"key: NaN", Yaml( mapping_t{ { u8"key", Yaml( u8"NaN" ) } } ) },
        { u8"key: 1_000", Yaml( mapping_t{ { u8"key", Yaml( u8"1_000" ) } } ) },
        { u8"key: 0b1010", Yaml( mapping_t{ { u8"key", Yaml( u8"0b1010" ) } } ) },
        { u8"key: 2026-09-06", Yaml( mapping_t{ { u8"key", Yaml( u8"2026-09-06" ) } } ) },

        // Quoted scalars keep their type and their spelling.
        { u8"key: \"42\"", Yaml( mapping_t{ { u8"key", Yaml( u8"42" ) } } ) },
        { u8"key: '42'", Yaml( mapping_t{ { u8"key", Yaml( u8"42" ) } } ) },
        { u8"key: \"\"", Yaml( mapping_t{ { u8"key", Yaml( u8"" ) } } ) },
        { u8"key: \"a\\nb\"", Yaml( mapping_t{ { u8"key", Yaml( u8"a\nb" ) } } ) },
        { u8"key: 'C:\\no\\escapes'", Yaml( mapping_t{ { u8"key", Yaml( u8"C:\\no\\escapes" ) } } ) },
        { u8"key: 'it''s'", Yaml( mapping_t{ { u8"key", Yaml( u8"it's" ) } } ) },
        { u8"'quoted key': 1", Yaml( mapping_t{ { u8"quoted key", Yaml( 1 ) } } ) },
        { u8"\"quoted key\": 1", Yaml( mapping_t{ { u8"quoted key", Yaml( 1 ) } } ) },

        // Plain scalars keep whatever the structural characters do not claim.
        { u8"a:b", Yaml( u8"a:b" ) },
        { u8"key: a:b", Yaml( mapping_t{ { u8"key", Yaml( u8"a:b" ) } } ) },
        { u8"url: http://example.com", Yaml( mapping_t{ { u8"url", Yaml( u8"http://example.com" ) } } ) },
        { u8"key: a, b", Yaml( mapping_t{ { u8"key", Yaml( u8"a, b" ) } } ) },
        { u8"key: a - b", Yaml( mapping_t{ { u8"key", Yaml( u8"a - b" ) } } ) },
        { u8"key: value with spaces   ", Yaml( mapping_t{ { u8"key", Yaml( u8"value with spaces" ) } } ) },
        { u8"key: b: c", Yaml( mapping_t{ { u8"key", Yaml( u8"b: c" ) } } ) },

        // Document markers; only the first document of a stream is parsed.
        { u8"---\nkey: 1", Yaml( mapping_t{ { u8"key", Yaml( 1 ) } } ) },
        { u8"--- hello", Yaml( u8"hello" ) },
        { u8"key: 1\n...", Yaml( mapping_t{ { u8"key", Yaml( 1 ) } } ) },
        { u8"---\nkey: 1\n...\n", Yaml( mapping_t{ { u8"key", Yaml( 1 ) } } ) },
        { u8"a: 1\n---\nb: 2", Yaml( mapping_t{ { u8"a", Yaml( 1 ) } } ) },
        { u8"- 1\n---\n- 2", Yaml( sequence_t{ Yaml( 1 ) } ) },

        // A duplicate key replaces the previous value.
        { u8"a: 1\na: 2", Yaml( mapping_t{ { u8"a", Yaml( 2 ) } } ) },

        // Robustness: none of the following is valid YAML, but all of it is read as
        // leniently as possible (with a diagnostic on the side).
        { u8"key: 'unterminated", Yaml( mapping_t{ { u8"key", Yaml( u8"'unterminated" ) } } ) },
        { u8"a: 1\n   b: 2", Yaml( mapping_t{ { u8"a", Yaml( 1 ) }, { u8"b", Yaml( 2 ) } } ) },
        { u8"\tkey: 1", Yaml( mapping_t{ { u8"key", Yaml( 1 ) } } ) },

        // What is recognizably not a plain scalar, but is not supported (yet)
        // either, ends up as a plain scalar rather than being dropped.
        { u8"key: [1, 2]", Yaml( mapping_t{ { u8"key", Yaml( u8"[1, 2]" ) } } ) },
        { u8"key: {a: 1}", Yaml( mapping_t{ { u8"key", Yaml( u8"{a: 1}" ) } } ) },
        { u8"key: &anchor value", Yaml( mapping_t{ { u8"key", Yaml( u8"&anchor value" ) } } ) },
        { u8"key: !!str 1", Yaml( mapping_t{ { u8"key", Yaml( u8"!!str 1" ) } } ) },

        {
u8"# a small config\n"
"title: Example\n"
"\n"
"owner:\n"
"  name: Tom\n"
"  age: 42\n"
"\n"
"database:\n"
"  enabled: true\n"
"  ports:\n"
"    - 8000\n"
"    - 8001\n"
"  limits:\n"
"    memory: 512\n"
"\r\n"
"crlf: 1\r\n",
            Yaml( mapping_t{
                { u8"title", Yaml( u8"Example" ) },
                { u8"owner", Yaml( mapping_t{ { u8"name", Yaml( u8"Tom" ) }, { u8"age", Yaml( 42 ) } } ) },
                { u8"database", Yaml( mapping_t{
                    { u8"enabled", Yaml( true ) },
                    { u8"ports", Yaml( sequence_t{ Yaml( 8000 ), Yaml( 8001 ) } ) },
                    { u8"limits", Yaml( mapping_t{ { u8"memory", Yaml( 512 ) } } ) },
                } ) },
                { u8"crlf", Yaml( 1 ) },
            } )
        },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.string )
    {
        const auto yaml = dl7::yaml::YamlReader::parse( entry.string );
        TESTLABS_CHECK_EQ( yaml, entry.yaml );
    }
}

TESTLABS_CASE( u8"DataLabs:  yaml:  YamlReader:  parse (not-a-number scalars)" )
{
    // NaN never compares equal, not even to itself, so it cannot take part in the
    // batch above.
    const auto yaml = dl7::yaml::YamlReader::parse( u8"key: .nan" );

    TESTLABS_CHECK( yaml.is_mapping() );
    TESTLABS_CHECK( yaml.at( u8"key" ).is_float() );
    TESTLABS_CHECK( std::isnan( yaml.at( u8"key" ).as_float() ) );
}
