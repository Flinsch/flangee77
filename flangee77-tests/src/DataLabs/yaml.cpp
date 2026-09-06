
#include <TestLabs/TestSuite.h>

#include <DataLabs/yaml/Yaml.h>
#include <DataLabs/yaml/YamlReader.h>
#include <DataLabs/yaml/YamlWriter.h>
#include <DataLabs/yaml/util/Unescaper.h>

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



TESTLABS_CASE( u8"DataLabs:  yaml:  util::Unescaper::unescape_string" )
{
    struct Entry
    {
        cl7::u8string yaml_string;
        cl7::u8string expected_string;
    } entry;

    const std::vector<Entry> container {
        { u8"", u8"" },
        { u8"Hello World", u8"Hello World" },

        // The escapes YAML shares with JSON.
        { u8"Hello\\nWorld", u8"Hello\nWorld" },
        { u8"Quote: \\\"", u8"Quote: \"" },
        { u8"\\t\\r\\n\\\\", u8"\t\r\n\\" },
        { u8"\\u00e4", u8"\u00e4" },
        { u8"\\uD83D\\uDE10", u8"\U0001f610" },

        // The ones it does not.
        { u8"\\a", u8"\a" },
        { u8"\\e", u8"\x1b" },
        { u8"\\N", u8"\u0085" },
        { u8"\\_", u8"\u00a0" },
        { u8"\\L", u8"\u2028" },
        { u8"\\P", u8"\u2029" },
        { u8"a\\ b", u8"a b" },
        { u8"\\U0001F610", u8"\U0001f610" },
        { u8"\\U000000e4", u8"\u00e4" },

        // An escaped backslash stays one, so what follows it is not an escape.
        { u8"\\\\N", u8"\\N" },
        { u8"\\\\\\N", u8"\\\u0085" },

        // What is not a valid escape sequence is left as it is.
        { u8"\\U0001F6", u8"\\U0001F6" },
        { u8"\\U0011FFFF", u8"\\U0011FFFF" },
        { u8"\\yz", u8"\\yz" },
        { u8"\\", u8"\\" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.yaml_string )
    {
        TESTLABS_CHECK_EQ( dl7::yaml::util::Unescaper{}.unescape_string( entry.yaml_string ), entry.expected_string );
    }
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

        // What is recognizably not a plain scalar, but is not supported either,
        // ends up as a plain scalar rather than being dropped.
        { u8"key: &anchor value", Yaml( mapping_t{ { u8"key", Yaml( u8"&anchor value" ) } } ) },
        { u8"key: !!str 1", Yaml( mapping_t{ { u8"key", Yaml( u8"!!str 1" ) } } ) },

        // Flow collections.
        { u8"key: []", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{} ) } } ) },
        { u8"key: {}", Yaml( mapping_t{ { u8"key", Yaml( mapping_t{} ) } } ) },
        { u8"key: [1, 2, 3]", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ), Yaml( 3 ) } ) } } ) },
        { u8"key: [1,2]", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) } } ) },
        { u8"key: [ 1 , 2 ]", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) } } ) },
        { u8"key: [1, ]", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( 1 ) } ) } } ) },
        { u8"key: [a, \"b\", 'c']", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( u8"a" ), Yaml( u8"b" ), Yaml( u8"c" ) } ) } } ) },
        { u8"key: [null, ~, true]", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml(), Yaml(), Yaml( true ) } ) } } ) },
        // A plain scalar may contain spaces inside a flow collection, too.
        { u8"key: [a b, c]", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( u8"a b" ), Yaml( u8"c" ) } ) } } ) },
        { u8"key: {a: 1, b: two}", Yaml( mapping_t{ { u8"key", Yaml( mapping_t{ { u8"a", Yaml( 1 ) }, { u8"b", Yaml( u8"two" ) } } ) } } ) },
        // A flow mapping key may have no value at all.
        { u8"key: {a: 1, b}", Yaml( mapping_t{ { u8"key", Yaml( mapping_t{ { u8"a", Yaml( 1 ) }, { u8"b", Yaml() } } ) } } ) },
        { u8"key: {\"a b\": 1}", Yaml( mapping_t{ { u8"key", Yaml( mapping_t{ { u8"a b", Yaml( 1 ) } } ) } } ) },
        { u8"key: [[1, 2], [3]]", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ), Yaml( sequence_t{ Yaml( 3 ) } ) } ) } } ) },
        { u8"key: {a: [1, 2], b: {c: 3}}", Yaml( mapping_t{ { u8"key", Yaml( mapping_t{ { u8"a", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) }, { u8"b", Yaml( mapping_t{ { u8"c", Yaml( 3 ) } } ) } } ) } } ) },
        { u8"key: [1, 2] # comment", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) } } ) },
        { u8"[1, 2]", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) },
        { u8"{a: 1}", Yaml( mapping_t{ { u8"a", Yaml( 1 ) } } ) },
        { u8"- [1, 2]\n- [3]", Yaml( sequence_t{ Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ), Yaml( sequence_t{ Yaml( 3 ) } ) } ) },
        // A flow collection may span as many lines as it likes.
        { u8"key: [\n  1,\n  2,\n]", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) } } ) },
        { u8"key: {\n  a: 1,\n  b: 2\n}", Yaml( mapping_t{ { u8"key", Yaml( mapping_t{ { u8"a", Yaml( 1 ) }, { u8"b", Yaml( 2 ) } } ) } } ) },
        { u8"key: [\n  1,\n  2,\n]\nother: 3", Yaml( mapping_t{ { u8"key", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) }, { u8"other", Yaml( 3 ) } } ) },

        // Block scalars.
        { u8"key: |\n  line1\n  line2", Yaml( mapping_t{ { u8"key", Yaml( u8"line1\nline2\n" ) } } ) },
        { u8"key: |-\n  line1\n  line2", Yaml( mapping_t{ { u8"key", Yaml( u8"line1\nline2" ) } } ) },
        { u8"key: |+\n  line1\n\n", Yaml( mapping_t{ { u8"key", Yaml( u8"line1\n\n" ) } } ) },
        { u8"key: |\n  a\n\n  b", Yaml( mapping_t{ { u8"key", Yaml( u8"a\n\nb\n" ) } } ) },
        { u8"key: |\n    indented\n      more", Yaml( mapping_t{ { u8"key", Yaml( u8"indented\n  more\n" ) } } ) },
        { u8"key: |2\n    text", Yaml( mapping_t{ { u8"key", Yaml( u8"  text\n" ) } } ) },
        // Inside a block scalar, a `#` is content like any other character.
        { u8"key: |\n  # not a comment", Yaml( mapping_t{ { u8"key", Yaml( u8"# not a comment\n" ) } } ) },
        // Folding turns a single line break into a space, n consecutive ones into
        // n-1 line breaks.
        { u8"key: >\n  line1\n  line2", Yaml( mapping_t{ { u8"key", Yaml( u8"line1 line2\n" ) } } ) },
        { u8"key: >-\n  a\n  b\n\n  c", Yaml( mapping_t{ { u8"key", Yaml( u8"a b\nc" ) } } ) },
        // A block scalar ends where the indentation does.
        { u8"key: |\n  a\nother: 1", Yaml( mapping_t{ { u8"key", Yaml( u8"a\n" ) }, { u8"other", Yaml( 1 ) } } ) },
        { u8"key: |\n  a\n\nother: 1", Yaml( mapping_t{ { u8"key", Yaml( u8"a\n" ) }, { u8"other", Yaml( 1 ) } } ) },
        { u8"key: |\n  a\n# comment\nother: 1", Yaml( mapping_t{ { u8"key", Yaml( u8"a\n" ) }, { u8"other", Yaml( 1 ) } } ) },
        { u8"- |\n  a\n- b", Yaml( sequence_t{ Yaml( u8"a\n" ), Yaml( u8"b" ) } ) },
        { u8"a:\n  b: |\n    text\n  c: 1", Yaml( mapping_t{ { u8"a", Yaml( mapping_t{ { u8"b", Yaml( u8"text\n" ) }, { u8"c", Yaml( 1 ) } } ) } } ) },

        // Escape sequences of double-quoted scalars, including the ones YAML has
        // beyond JSON's.
        { u8"key: \"a\\tb\"", Yaml( mapping_t{ { u8"key", Yaml( u8"a\tb" ) } } ) },
        { u8"key: \"a\\\"b\"", Yaml( mapping_t{ { u8"key", Yaml( u8"a\"b" ) } } ) },
        { u8"key: \"\\u00e4\"", Yaml( mapping_t{ { u8"key", Yaml( u8"\u00e4" ) } } ) },
        { u8"key: \"\\U0001F600\"", Yaml( mapping_t{ { u8"key", Yaml( u8"\U0001F600" ) } } ) },
        { u8"key: \"\\N\"", Yaml( mapping_t{ { u8"key", Yaml( u8"\u0085" ) } } ) },
        { u8"key: \"\\_\"", Yaml( mapping_t{ { u8"key", Yaml( u8"\u00a0" ) } } ) },
        { u8"key: \"\\L\"", Yaml( mapping_t{ { u8"key", Yaml( u8"\u2028" ) } } ) },
        { u8"key: \"a\\ b\"", Yaml( mapping_t{ { u8"key", Yaml( u8"a b" ) } } ) },
        // An escaped backslash stays one, so what follows is not an escape.
        { u8"key: \"\\\\N\"", Yaml( mapping_t{ { u8"key", Yaml( u8"\\N" ) } } ) },
        { u8"key: \"\\\\\"", Yaml( mapping_t{ { u8"key", Yaml( u8"\\" ) } } ) },

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



TESTLABS_CASE( u8"DataLabs:  yaml:  YamlWriter:  to_string" )
{
    using dl7::yaml::Yaml;
    using dl7::yaml::mapping_t;
    using dl7::yaml::sequence_t;

    struct Entry
    {
        cl7::u8string label;
        Yaml yaml;
        cl7::u8string expected_string;
    } entry;

    const std::vector<Entry> container {
        // A document may be a scalar all by itself.
        { u8"null", Yaml(), u8"null\n" },
        { u8"string", Yaml( u8"hello" ), u8"hello\n" },
        { u8"integer", Yaml( 42 ), u8"42\n" },

        // Block mappings and sequences.
        { u8"mapping", Yaml( mapping_t{ { u8"a", Yaml( 1 ) }, { u8"b", Yaml( u8"two" ) } } ), u8"a: 1\nb: two\n" },
        { u8"nested mapping", Yaml( mapping_t{ { u8"a", Yaml( mapping_t{ { u8"b", Yaml( 1 ) } } ) } } ), u8"a:\n  b: 1\n" },
        { u8"sequence", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ), u8"- 1\n- 2\n" },
        // A block sequence sits at its parent key's indentation by default.
        { u8"sequence value", Yaml( mapping_t{ { u8"a", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) } } ), u8"a:\n- 1\n- 2\n" },
        // Mappings and sequences inside a sequence use the compact notation.
        { u8"mapping in sequence", Yaml( sequence_t{ Yaml( mapping_t{ { u8"a", Yaml( 1 ) }, { u8"b", Yaml( 2 ) } } ) } ), u8"- a: 1\n  b: 2\n" },
        { u8"sequence in sequence", Yaml( sequence_t{ Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ) } ), u8"- - 1\n  - 2\n" },
        // An empty collection has no block representation of its own.
        { u8"empty mapping value", Yaml( mapping_t{ { u8"a", Yaml( mapping_t{} ) } } ), u8"a: {}\n" },
        { u8"empty sequence value", Yaml( mapping_t{ { u8"a", Yaml( sequence_t{} ) } } ), u8"a: []\n" },

        // Scalars.
        { u8"null value", Yaml( mapping_t{ { u8"a", Yaml() } } ), u8"a: null\n" },
        { u8"boolean value", Yaml( mapping_t{ { u8"a", Yaml( true ) } } ), u8"a: true\n" },
        { u8"float value", Yaml( mapping_t{ { u8"a", Yaml( 7.5 ) } } ), u8"a: 7.5\n" },
        { u8"whole float value", Yaml( mapping_t{ { u8"a", Yaml( 7.0 ) } } ), u8"a: 7.0\n" },
        { u8"infinity", Yaml( mapping_t{ { u8"a", Yaml( std::numeric_limits<double>::infinity() ) } } ), u8"a: .inf\n" },
        { u8"plain string value", Yaml( mapping_t{ { u8"a", Yaml( u8"hello world" ) } } ), u8"a: hello world\n" },

        // A string is quoted exactly where it would otherwise be read back as
        // something other than that very string.
        { u8"string that looks like a boolean", Yaml( mapping_t{ { u8"a", Yaml( u8"true" ) } } ), u8"a: \"true\"\n" },
        { u8"string that looks like a number", Yaml( mapping_t{ { u8"a", Yaml( u8"7" ) } } ), u8"a: \"7\"\n" },
        { u8"string that looks like null", Yaml( mapping_t{ { u8"a", Yaml( u8"~" ) } } ), u8"a: \"~\"\n" },
        { u8"empty string", Yaml( mapping_t{ { u8"a", Yaml( u8"" ) } } ), u8"a: \"\"\n" },
        { u8"string with a key separator", Yaml( mapping_t{ { u8"a", Yaml( u8"x: y" ) } } ), u8"a: \"x: y\"\n" },
        { u8"string with a comment", Yaml( mapping_t{ { u8"a", Yaml( u8"x # y" ) } } ), u8"a: \"x # y\"\n" },
        { u8"string with a leading indicator", Yaml( mapping_t{ { u8"a", Yaml( u8"- x" ) } } ), u8"a: \"- x\"\n" },
        { u8"string with surrounding space", Yaml( mapping_t{ { u8"a", Yaml( u8" x " ) } } ), u8"a: \" x \"\n" },
        { u8"document marker as a string", Yaml( mapping_t{ { u8"a", Yaml( u8"---" ) } } ), u8"a: \"---\"\n" },

        // Keys follow the very same rules.
        { u8"plain key with a space", Yaml( mapping_t{ { u8"a b", Yaml( 1 ) } } ), u8"a b: 1\n" },
        { u8"key that needs quoting", Yaml( mapping_t{ { u8"a: b", Yaml( 1 ) } } ), u8"\"a: b\": 1\n" },

        // A multi-line string becomes a literal block scalar, the chomping
        // indicator following from its trailing line breaks.
        { u8"block scalar", Yaml( mapping_t{ { u8"a", Yaml( u8"line1\nline2\n" ) } } ), u8"a: |\n  line1\n  line2\n" },
        { u8"block scalar without a trailing line break", Yaml( mapping_t{ { u8"a", Yaml( u8"line1\nline2" ) } } ), u8"a: |-\n  line1\n  line2\n" },
        { u8"block scalar with trailing line breaks", Yaml( mapping_t{ { u8"a", Yaml( u8"line1\n\n" ) } } ), u8"a: |+\n  line1\n\n" },
        { u8"block scalar with an empty line", Yaml( mapping_t{ { u8"a", Yaml( u8"a\n\nb\n" ) } } ), u8"a: |\n  a\n\n  b\n" },
        // Indented content needs an explicit indentation indicator, which would
        // otherwise be read off the first line.
        { u8"block scalar with indented content", Yaml( mapping_t{ { u8"a", Yaml( u8"  x\ny\n" ) } } ), u8"a: |2\n    x\n  y\n" },
        // What a block scalar cannot hold stays a quoted one-liner.
        { u8"multi-line string with a tab", Yaml( mapping_t{ { u8"a", Yaml( u8"x\n\ty\n" ) } } ), u8"a: \"x\\n\\ty\\n\"\n" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.label )
    {
        TESTLABS_CHECK_EQ( dl7::yaml::YamlWriter::to_string( entry.yaml ), entry.expected_string );
    }
}

TESTLABS_CASE( u8"DataLabs:  yaml:  YamlWriter:  to_string (flow style)" )
{
    using dl7::yaml::Yaml;
    using dl7::yaml::mapping_t;
    using dl7::yaml::sequence_t;

    struct Entry
    {
        cl7::u8string label;
        Yaml yaml;
        cl7::u8string expected_string;
    } entry;

    const std::vector<Entry> container {
        { u8"null", Yaml(), u8"null" },
        { u8"string", Yaml( u8"hello" ), u8"hello" },
        { u8"empty sequence", Yaml( sequence_t{} ), u8"[]" },
        { u8"empty mapping", Yaml( mapping_t{} ), u8"{}" },
        { u8"sequence", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ) } ), u8"[1, 2]" },
        { u8"mapping", Yaml( mapping_t{ { u8"a", Yaml( 1 ) }, { u8"b", Yaml( u8"two" ) } } ), u8"{a: 1, b: two}" },
        { u8"nested", Yaml( mapping_t{ { u8"a", Yaml( sequence_t{ Yaml( 1 ), Yaml( mapping_t{ { u8"b", Yaml( 2 ) } } ) } ) } } ), u8"{a: [1, {b: 2}]}" },
        // Inside a flow collection, its own punctuation forces quotes.
        { u8"string with a comma", Yaml( mapping_t{ { u8"a", Yaml( u8"x,y" ) } } ), u8"{a: \"x,y\"}" },
        { u8"string with a bracket", Yaml( mapping_t{ { u8"a", Yaml( u8"x]y" ) } } ), u8"{a: \"x]y\"}" },
        // ... but a plain scalar may still contain spaces.
        { u8"string with a space", Yaml( mapping_t{ { u8"a", Yaml( u8"x y" ) } } ), u8"{a: x y}" },
        // A multi-line string cannot be a block scalar here.
        { u8"multi-line string", Yaml( mapping_t{ { u8"a", Yaml( u8"x\ny" ) } } ), u8"{a: \"x\\ny\"}" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.label )
    {
        TESTLABS_CHECK_EQ( dl7::yaml::YamlWriter::to_string( entry.yaml, dl7::yaml::YamlWriter::DEFAULT_FLOW_FORMAT ), entry.expected_string );
    }
}

TESTLABS_CASE( u8"DataLabs:  yaml:  YamlWriter:  to_string (format options)" )
{
    using dl7::yaml::Yaml;
    using dl7::yaml::Format;
    using dl7::yaml::mapping_t;
    using dl7::yaml::sequence_t;

    const Yaml yaml( mapping_t{
        { u8"a", Yaml( mapping_t{ { u8"b", Yaml( 1 ) } } ) },
        { u8"c", Yaml( sequence_t{ Yaml( 2 ) } ) },
        { u8"d", Yaml() },
        { u8"e", Yaml( u8"x" ) },
    } );

    struct Entry
    {
        cl7::u8string label;
        Format format;
        cl7::u8string expected_string;
    } entry;

    const std::vector<Entry> container {
        { u8"defaults", {}, u8"a:\n  b: 1\nc:\n- 2\nd: null\ne: x\n" },
        { u8"four spaces", { .indentation = Format::Indentation::Spaces4 }, u8"a:\n    b: 1\nc:\n- 2\nd: null\ne: x\n" },
        { u8"CRLF", { .line_ending = Format::LineEnding::CRLF }, u8"a:\r\n  b: 1\r\nc:\r\n- 2\r\nd: null\r\ne: x\r\n" },
        { u8"indented sequences", { .sequence_indentation = Format::SequenceIndentation::OneLevel }, u8"a:\n  b: 1\nc:\n  - 2\nd: null\ne: x\n" },
        { u8"single-quoted scalars", { .scalar_quoting = Format::ScalarQuoting::SingleQuoted }, u8"'a':\n  'b': 1\n'c':\n- 2\n'd': null\n'e': 'x'\n" },
        { u8"null as a tilde", { .null_representation = Format::NullRepresentation::Tilde }, u8"a:\n  b: 1\nc:\n- 2\nd: ~\ne: x\n" },
        { u8"null as nothing", { .null_representation = Format::NullRepresentation::Empty }, u8"a:\n  b: 1\nc:\n- 2\nd:\ne: x\n" },
        { u8"explicit document markers", { .explicit_document_start = true, .explicit_document_end = true }, u8"---\na:\n  b: 1\nc:\n- 2\nd: null\ne: x\n...\n" },
        { u8"trailing empty line", { .add_empty_line = true }, u8"a:\n  b: 1\nc:\n- 2\nd: null\ne: x\n\n" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.label )
    {
        TESTLABS_CHECK_EQ( dl7::yaml::YamlWriter::to_string( yaml, entry.format ), entry.expected_string );
    }
}

TESTLABS_CASE( u8"DataLabs:  yaml:  round trip (parse after write)" )
{
    using dl7::yaml::Yaml;
    using dl7::yaml::mapping_t;
    using dl7::yaml::sequence_t;

    struct Entry
    {
        cl7::u8string label;
        Yaml yaml;
    } entry;

    const std::vector<Entry> container {
        { u8"null", Yaml() },
        { u8"scalar", Yaml( u8"hello" ) },
        { u8"mapping", Yaml( mapping_t{ { u8"a", Yaml( 1 ) }, { u8"b", Yaml( u8"two" ) } } ) },
        { u8"sequence", Yaml( sequence_t{ Yaml( 1 ), Yaml( 2 ), Yaml( 3 ) } ) },
        { u8"nested", Yaml( mapping_t{ { u8"a", Yaml( mapping_t{ { u8"b", Yaml( sequence_t{ Yaml( 1 ), Yaml( mapping_t{ { u8"c", Yaml( true ) } } ) } ) } } ) } } ) },
        { u8"empty collections", Yaml( mapping_t{ { u8"a", Yaml( mapping_t{} ) }, { u8"b", Yaml( sequence_t{} ) } } ) },
        { u8"nulls", Yaml( mapping_t{ { u8"a", Yaml() }, { u8"b", Yaml( sequence_t{ Yaml(), Yaml() } ) } } ) },
        { u8"numbers", Yaml( mapping_t{ { u8"a", Yaml( -7 ) }, { u8"b", Yaml( 7.5 ) }, { u8"c", Yaml( 0.0 ) }, { u8"d", Yaml( std::numeric_limits<double>::infinity() ) } } ) },

        // The strings that make plain scalars interesting.
        { u8"string that looks like a number", Yaml( mapping_t{ { u8"a", Yaml( u8"7" ) } } ) },
        { u8"string that looks like a boolean", Yaml( mapping_t{ { u8"a", Yaml( u8"yes" ) }, { u8"b", Yaml( u8"true" ) } } ) },
        { u8"empty string", Yaml( mapping_t{ { u8"a", Yaml( u8"" ) } } ) },
        { u8"document markers", Yaml( mapping_t{ { u8"a", Yaml( u8"---" ) }, { u8"b", Yaml( u8"..." ) } } ) },
        { u8"key separator", Yaml( mapping_t{ { u8"a", Yaml( u8"x: y" ) }, { u8"b", Yaml( u8"x:y" ) } } ) },
        { u8"comment", Yaml( mapping_t{ { u8"a", Yaml( u8"x # y" ) }, { u8"b", Yaml( u8"x#y" ) } } ) },
        { u8"surrounding space", Yaml( mapping_t{ { u8"a", Yaml( u8" x" ) }, { u8"b", Yaml( u8"x " ) } } ) },
        { u8"indicators", Yaml( mapping_t{ { u8"a", Yaml( u8"- x" ) }, { u8"b", Yaml( u8"[1]" ) }, { u8"c", Yaml( u8"{a}" ) }, { u8"d", Yaml( u8"*x" ) } } ) },
        { u8"quotes", Yaml( mapping_t{ { u8"a", Yaml( u8"x \"y\" z" ) }, { u8"b", Yaml( u8"it's" ) } } ) },
        { u8"backslash", Yaml( mapping_t{ { u8"a", Yaml( u8"C:\\path" ) } } ) },
        { u8"tab", Yaml( mapping_t{ { u8"a", Yaml( u8"x\ty" ) } } ) },
        { u8"non-ASCII", Yaml( mapping_t{ { u8"a", Yaml( u8"\u00e4\u00f6\u00fc" ) }, { u8"b", Yaml( u8"\U0001F600" ) } } ) },
        { u8"keys that need quoting", Yaml( mapping_t{ { u8"a: b", Yaml( 1 ) }, { u8"", Yaml( 2 ) }, { u8"7", Yaml( 3 ) } } ) },

        // Multi-line strings, i.e., block scalars.
        { u8"multi-line string", Yaml( mapping_t{ { u8"a", Yaml( u8"line1\nline2\n" ) } } ) },
        { u8"multi-line string without a trailing line break", Yaml( mapping_t{ { u8"a", Yaml( u8"line1\nline2" ) } } ) },
        { u8"multi-line string with trailing line breaks", Yaml( mapping_t{ { u8"a", Yaml( u8"line1\n\n\n" ) } } ) },
        { u8"multi-line string with an empty line", Yaml( mapping_t{ { u8"a", Yaml( u8"a\n\nb\n" ) } } ) },
        { u8"multi-line string with indented content", Yaml( mapping_t{ { u8"a", Yaml( u8"  x\ny\n" ) } } ) },
        { u8"multi-line string with a tab", Yaml( mapping_t{ { u8"a", Yaml( u8"x\n\ty\n" ) } } ) },
        { u8"multi-line string in a sequence", Yaml( sequence_t{ Yaml( u8"a\nb\n" ), Yaml( u8"c" ) } ) },

        {
            u8"mixed",
            Yaml( mapping_t{
                { u8"title", Yaml( u8"Example" ) },
                { u8"owner", Yaml( mapping_t{ { u8"name", Yaml( u8"Tom" ) }, { u8"age", Yaml( 42 ) } } ) },
                { u8"database", Yaml( mapping_t{
                    { u8"enabled", Yaml( true ) },
                    { u8"ports", Yaml( sequence_t{ Yaml( 8000 ), Yaml( 8001 ) } ) },
                    { u8"hosts", Yaml( sequence_t{ Yaml( mapping_t{ { u8"host", Yaml( u8"a" ) }, { u8"port", Yaml( 1 ) } } ) } ) },
                } ) },
            } )
        },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.label )
    {
        const auto string = dl7::yaml::YamlWriter::to_string( entry.yaml );
        TESTLABS_CHECK_EQ( dl7::yaml::YamlReader::parse( string ), entry.yaml );

        const auto flow_string = dl7::yaml::YamlWriter::to_string( entry.yaml, dl7::yaml::YamlWriter::DEFAULT_FLOW_FORMAT );
        TESTLABS_CHECK_EQ( dl7::yaml::YamlReader::parse( flow_string ), entry.yaml );
    }
}
