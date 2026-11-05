
#include <TestLabs/TestSuite.h>

#include <FaceLabs/fonts/Icon.h>
#include <FaceLabs/fonts/IconRun.h>
#include <FaceLabs/fonts/TextLayout.h>
#include <FaceLabs/fonts/detail/ttf/TrueTypeFontLoader.h>

#include "../../shared.h"

#include <CoreLabs/platform/filesystem.h>

#include <span>
#include <string>
#include <string_view>
#include <vector>



namespace {

    std::vector<cl7::text::codec::codepoint> to_codepoints(std::string_view s)
    {
        std::vector<cl7::text::codec::codepoint> codepoints;
        codepoints.reserve(s.size());
        for (char c : s)
            codepoints.push_back(cl7::text::codec::codepoint{static_cast<unsigned>(static_cast<unsigned char>(c))});
        return codepoints;
    }

    std::string to_string(std::span<const cl7::text::codec::codepoint> codepoints, const fl7::fonts::TextLine& line)
    {
        std::string s;
        for (size_t i = line.codepoint_begin; i < line.codepoint_end; ++i)
            s.push_back(static_cast<char>(codepoints[i].value));
        return s;
    }

    fl7::fonts::Font& get_test_font()
    {
        static fl7::fonts::Font font(std::make_unique<fl7::fonts::detail::ttf::TrueTypeFontLoader>(
            cl7::platform::filesystem::get_working_directory() + u8"assets/fonts/Noto/NotoSans-Regular.ttf"));
        return font;
    }

    float measure(std::string_view s, fl7::fonts::Font& font, const fl7::fonts::TextStyle& text_style)
    {
        fl7::fonts::TextStyle unwrapped_style = text_style;
        unwrapped_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::None;
        const auto codepoints = to_codepoints(s);
        const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, unwrapped_style, 0.0f);
        return lines.at(0).width;
    }

} // namespace



TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - explicit line breaks (WrapMode::None)" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;

    const auto codepoints = to_codepoints("AAA\nBB\r\nC");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, 0.0f);

    TESTLABS_CHECK_EQ( lines.size(), size_t{3} );
    TESTLABS_CHECK_EQ( to_string(codepoints, lines.at(0)), "AAA" );
    TESTLABS_CHECK_EQ( to_string(codepoints, lines.at(1)), "BB" );
    TESTLABS_CHECK_EQ( to_string(codepoints, lines.at(2)), "C" );

    // Each line is a single-line paragraph of its own.
    for (const auto& line : lines)
    {
        TESTLABS_CHECK( line.is_paragraph_end );
        TESTLABS_CHECK_EQ( line.word_count, 1u );
    }
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - explicit line breaks are ignored when max_width is unset" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;
    text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::Word;

    const auto codepoints = to_codepoints("aaa bbb ccc");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, 0.0f);

    TESTLABS_CHECK_EQ( lines.size(), size_t{1} );
    TESTLABS_CHECK_EQ( to_string(codepoints, lines.at(0)), "aaa bbb ccc" );
    TESTLABS_CHECK( lines.at(0).is_paragraph_end );
    TESTLABS_CHECK_EQ( lines.at(0).word_count, 3u );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - word-wrap across multiple lines" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;
    text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::Word;

    const float max_width = measure("aaa bbb", font, text_style);

    const auto codepoints = to_codepoints("aaa bbb ccc");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, max_width);

    TESTLABS_CHECK_EQ( lines.size(), size_t{2} );
    TESTLABS_CHECK_EQ( to_string(codepoints, lines.at(0)), "aaa bbb" );
    TESTLABS_CHECK_EQ( to_string(codepoints, lines.at(1)), "ccc" );
    TESTLABS_CHECK_LE( lines.at(0).width, max_width );

    TESTLABS_CHECK_EQ( lines.at(0).word_count, 2u );
    TESTLABS_CHECK( !lines.at(0).is_paragraph_end );
    TESTLABS_CHECK_EQ( lines.at(1).word_count, 1u );
    TESTLABS_CHECK( lines.at(1).is_paragraph_end );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - mid-word hard break for an overlong word" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;
    text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::Word;

    const float max_width = measure("abc", font, text_style);

    const std::string_view word = "abcdefghij";
    const auto codepoints = to_codepoints(word);
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, max_width);

    // The word doesn't fit on a single line, so it must be split into more than one.
    TESTLABS_CHECK_GT( lines.size(), size_t{1} );

    // Every fragment must stay within the box width, and the fragments must
    // cover the whole word contiguously, in order, without gaps or overlaps.
    // Each fragment is a piece of a single word, so word_count is always 1,
    // and only the final fragment ends its (one-line) paragraph.
    std::string reassembled;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        const auto& line = lines[i];
        TESTLABS_CHECK_LE( line.width, max_width );
        TESTLABS_CHECK_GT( line.codepoint_end, line.codepoint_begin );
        TESTLABS_CHECK_EQ( line.word_count, 1u );
        TESTLABS_CHECK_EQ( line.is_paragraph_end, i == lines.size() - 1 );
        reassembled += to_string(codepoints, line);
    }
    TESTLABS_CHECK_EQ( reassembled, std::string(word) );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - leading/trailing whitespace is trimmed" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;

    const auto codepoints = to_codepoints("   hello   ");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, 0.0f);

    TESTLABS_CHECK_EQ( lines.size(), size_t{1} );
    TESTLABS_CHECK_EQ( to_string(codepoints, lines.at(0)), "hello" );
    TESTLABS_CHECK_EQ_FLT( lines.at(0).width, measure("hello", font, text_style) );
    TESTLABS_CHECK_EQ( lines.at(0).word_count, 1u );
    TESTLABS_CHECK( lines.at(0).is_paragraph_end );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - preserve_whitespace keeps leading/trailing whitespace (WrapMode::None)" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;
    text_style.preserve_whitespace = true;

    const auto codepoints = to_codepoints("   hello   ");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, 0.0f);

    TESTLABS_CHECK_EQ( lines.size(), size_t{1} );
    TESTLABS_CHECK_EQ( to_string(codepoints, lines.at(0)), "   hello   " );
    TESTLABS_CHECK_EQ_FLT( lines.at(0).width, measure("   hello   ", font, text_style) );
    // The surrounding whitespace must actually contribute to the measured
    // width, not merely leave the range untrimmed while still measuring
    // only "hello".
    TESTLABS_CHECK_GT( lines.at(0).width, measure("hello", font, text_style) );
    TESTLABS_CHECK( lines.at(0).is_paragraph_end );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - preserve_whitespace keeps an all-whitespace paragraph (WrapMode::None)" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;
    text_style.preserve_whitespace = true;

    const auto codepoints = to_codepoints("   ");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, 0.0f);

    TESTLABS_CHECK_EQ( lines.size(), size_t{1} );
    TESTLABS_CHECK_EQ( lines.at(0).codepoint_begin, size_t{0} );
    TESTLABS_CHECK_EQ( lines.at(0).codepoint_end, codepoints.size() );
    TESTLABS_CHECK_GT( lines.at(0).width, 0.0f );
    TESTLABS_CHECK( lines.at(0).is_paragraph_end );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - preserve_whitespace still trims word-wrap boundaries (WrapMode::Word)" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;
    text_style.preserve_whitespace = true;
    text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::Word;

    // Generous max_width: the whole paragraph still fits on a single
    // (word-wrapped) line, so this exercises _word_wrap_paragraph() (whose
    // line boundaries are always word boundaries), not _make_line()'s own
    // trim_whitespace branch.
    const float max_width = measure("  aaa bbb  ", font, text_style) * 10.0f;

    const auto codepoints = to_codepoints("  aaa bbb  ");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, max_width);

    TESTLABS_CHECK_EQ( lines.size(), size_t{1} );
    TESTLABS_CHECK_EQ( to_string(codepoints, lines.at(0)), "aaa bbb" );
    TESTLABS_CHECK_EQ( lines.at(0).word_count, 2u );
    TESTLABS_CHECK( lines.at(0).is_paragraph_end );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - preserve_whitespace still collapses an all-whitespace paragraph (WrapMode::Word)" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;
    text_style.preserve_whitespace = true;
    text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::Word;

    const auto codepoints = to_codepoints("   ");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, 100.0f);

    TESTLABS_CHECK_EQ( lines.size(), size_t{1} );
    TESTLABS_CHECK_EQ( lines.at(0).codepoint_begin, lines.at(0).codepoint_end ); // collapsed to empty
    TESTLABS_CHECK_EQ_FLT( lines.at(0).width, 0.0f );
    TESTLABS_CHECK_EQ( lines.at(0).word_count, 0u );
    TESTLABS_CHECK( lines.at(0).is_paragraph_end );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - explicit line breaks still split paragraphs under WrapMode::Word" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;
    text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::Word;

    // A generous max_width that comfortably fits each paragraph on its own,
    // so only the explicit line break should cause a split, not word-wrap.
    const float max_width = measure("aaa bbb ccc ddd", font, text_style) * 10.0f;

    const auto codepoints = to_codepoints("aaa bbb\nccc ddd");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, max_width);

    TESTLABS_CHECK_EQ( lines.size(), size_t{2} );
    TESTLABS_CHECK_EQ( to_string(codepoints, lines.at(0)), "aaa bbb" );
    TESTLABS_CHECK_EQ( to_string(codepoints, lines.at(1)), "ccc ddd" );

    // Each explicit-break paragraph is its own single-line paragraph.
    TESTLABS_CHECK_EQ( lines.at(0).word_count, 2u );
    TESTLABS_CHECK( lines.at(0).is_paragraph_end );
    TESTLABS_CHECK_EQ( lines.at(1).word_count, 2u );
    TESTLABS_CHECK( lines.at(1).is_paragraph_end );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - letter_spacing adds spacing between every code point" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle base_style;
    fl7::fonts::TextStyle spaced_style;
    spaced_style.letter_spacing = 5.0f;

    const std::string_view text = "hello";
    const float base_width = measure(text, font, base_style);
    const float spaced_width = measure(text, font, spaced_style);

    TESTLABS_CHECK_EQ_ROUND( 3, spaced_width, base_width + spaced_style.letter_spacing * static_cast<float>(text.size() - 1) );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - word_spacing adds spacing once per whitespace run" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle base_style;
    fl7::fonts::TextStyle spaced_style;
    spaced_style.word_spacing = 7.0f;

    // Two interior whitespace runs (no leading/trailing whitespace, so
    // trimming in `measure()` doesn't interfere).
    const std::string_view text = "aaa   bbb ccc";
    const float base_width = measure(text, font, base_style);
    const float spaced_width = measure(text, font, spaced_style);

    TESTLABS_CHECK_EQ_ROUND( 3, spaced_width, base_width + spaced_style.word_spacing * 2.0f );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - letter_spacing and word_spacing shift word-wrap boundaries" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;
    text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::Word;

    const float max_width = measure("aaa bbb", font, text_style);

    fl7::fonts::TextStyle spaced_style = text_style;
    spaced_style.letter_spacing = 2.0f;
    spaced_style.word_spacing = 10.0f;

    const auto codepoints = to_codepoints("aaa bbb ccc");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, spaced_style, max_width);

    // With the extra spacing, "aaa bbb" no longer fits within max_width, so
    // the wrap boundary must shift earlier than the zero-spacing case (which
    // fits both words on line 1, per the word-wrap test above).
    TESTLABS_CHECK_NE( to_string(codepoints, lines.at(0)), "aaa bbb" );
    for (const auto& line : lines)
        TESTLABS_CHECK_LE( line.width, max_width );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - an icon run's width overrides the code point's glyph advance" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;

    const fl7::fonts::Icon icon{.size = {123.0f, 40.0f}};
    const fl7::fonts::IconRun icon_runs[] = {{.codepoint_index = 1, .icon = &icon}};

    // "a?b": the '?' placeholder at index 1 is entirely overridden by the icon.
    const auto codepoints = to_codepoints("a?b");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, 0.0f, icon_runs);

    const float expected_width = measure("a", font, text_style) + icon.size.x + measure("b", font, text_style);
    TESTLABS_CHECK_EQ( lines.size(), size_t{1} );
    TESTLABS_CHECK_EQ_FLT( lines.at(0).width, expected_width );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out - an icon-covered code point is never treated as whitespace" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;

    const fl7::fonts::Icon icon{.size = {20.0f, 20.0f}};
    // The placeholder happens to be a literal space, at the very start of the
    // text: if it were (incorrectly) treated as real whitespace, it would be
    // (a) trimmed away as leading whitespace and (b) not count towards a word.
    const fl7::fonts::IconRun icon_runs[] = {{.codepoint_index = 0, .icon = &icon}};

    const auto codepoints = to_codepoints(" ab");
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, 0.0f, icon_runs);

    TESTLABS_CHECK_EQ( lines.size(), size_t{1} );
    TESTLABS_CHECK_EQ( lines.at(0).codepoint_begin, size_t{0} ); // not trimmed away
    TESTLABS_CHECK_EQ( lines.at(0).word_count, 1u ); // icon + "ab" is one contiguous word
}



// #############################################################################
// locate() / lay_out_box() (gui::render::CaretHelper/SelectionHelper, and
// gui::faces::TextField's multiline support, all build on these)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  locate - a code point right before an explicit line break stays on that line; right after it starts the next one" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style; // preserve_whitespace = false by default, irrelevant here: no leading/trailing whitespace

    const auto codepoints = to_codepoints("AAA\nBBB"); // 'A'=0..2, '\n'=3, 'B'=4..6
    const ml7::Vector2f box_position{10.0f, 20.0f};
    const ml7::Vector2f box_size{200.0f, 200.0f};

    const auto at_very_start = fl7::fonts::TextLayout::locate(codepoints, 0, font, text_style, box_position, box_size);
    const auto at_end_of_first_line = fl7::fonts::TextLayout::locate(codepoints, 3, font, text_style, box_position, box_size); // right before '\n'
    const auto at_start_of_second_line = fl7::fonts::TextLayout::locate(codepoints, 4, font, text_style, box_position, box_size); // right after '\n'

    TESTLABS_CHECK_EQ_FLT( at_end_of_first_line.position.y, at_very_start.position.y ); // still line 0
    // Compared as a+b == a+b rather than (a+b)-a == b: the latter is only
    // accurate to within a magnitude-relative epsilon, not the fixed absolute
    // one TESTLABS_CHECK_EQ_FLT actually uses.
    TESTLABS_CHECK_EQ_FLT( at_start_of_second_line.position.y, at_end_of_first_line.position.y + at_end_of_first_line.line_height ); // one line down
    TESTLABS_CHECK_EQ_FLT( at_start_of_second_line.position.x, at_very_start.position.x ); // line 1 starts flush left, same as line 0

    const float aaa_width = fl7::fonts::TextLayout::measure_advance(std::span<const cl7::text::codec::codepoint>(codepoints).subspan(0, 3), font, text_style);
    TESTLABS_CHECK_EQ_FLT( at_end_of_first_line.position.x, at_very_start.position.x + aaa_width );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  locate - a code point right at a word-wrap boundary starts the wrapped continuation line, same as an explicit break" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;
    text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::Word;

    const float max_width = measure("aaa bbb", font, text_style); // just enough for one line

    const auto codepoints = to_codepoints("aaa bbb"); // 'a'=0..2, ' '=3, 'b'=4..6
    const auto lines = fl7::fonts::TextLayout::lay_out(codepoints, font, text_style, max_width * 0.5f); // forces a wrap between the two words
    TESTLABS_ASSERT( lines.size() == 2 );

    const ml7::Vector2f box_position{0.0f, 0.0f};
    const ml7::Vector2f box_size{max_width * 0.5f, 200.0f};

    const auto at_very_start = fl7::fonts::TextLayout::locate(codepoints, 0, font, text_style, box_position, box_size);
    const auto at_end_of_first_word = fl7::fonts::TextLayout::locate(codepoints, 3, font, text_style, box_position, box_size); // right after "aaa", at the wrap space
    const auto at_start_of_second_word = fl7::fonts::TextLayout::locate(codepoints, 4, font, text_style, box_position, box_size); // right at "bbb"

    TESTLABS_CHECK_EQ_FLT( at_end_of_first_word.position.y, at_very_start.position.y ); // still line 0
    TESTLABS_CHECK_EQ_FLT( at_start_of_second_word.position.y, at_end_of_first_word.position.y + at_end_of_first_word.line_height ); // one line down
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  lay_out_box - vertical_align positions the whole block within a taller box, Top above Bottom" )
{
    auto& font = get_test_font();

    fl7::fonts::TextStyle top_style;
    top_style.vertical_align = fl7::fonts::TextStyle::VerticalAlign::Top;
    fl7::fonts::TextStyle bottom_style = top_style;
    bottom_style.vertical_align = fl7::fonts::TextStyle::VerticalAlign::Bottom;

    const auto codepoints = to_codepoints("AAA\nBBB"); // two lines, nowhere near filling the box below
    const ml7::Vector2f box_position{0.0f, 0.0f};
    const ml7::Vector2f box_size{200.0f, 500.0f};

    const auto top_block = fl7::fonts::TextLayout::lay_out_box(codepoints, font, top_style, box_position, box_size);
    const auto bottom_block = fl7::fonts::TextLayout::lay_out_box(codepoints, font, bottom_style, box_position, box_size);

    TESTLABS_CHECK_EQ( top_block.lines.size(), bottom_block.lines.size() );
    TESTLABS_CHECK_LT( top_block.top, bottom_block.top ); // Top starts higher up (smaller y) than Bottom
    TESTLABS_CHECK_EQ_FLT( top_block.line_height, bottom_block.line_height ); // same font/style: same line height regardless
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  TextLayout:  locate - a code point index past the end of the text clamps to the very end" )
{
    auto& font = get_test_font();
    fl7::fonts::TextStyle text_style;

    const auto codepoints = to_codepoints("AAA\nBBB");
    const ml7::Vector2f box_position{0.0f, 0.0f};
    const ml7::Vector2f box_size{200.0f, 200.0f};

    const auto at_actual_end = fl7::fonts::TextLayout::locate(codepoints, codepoints.size(), font, text_style, box_position, box_size);
    const auto at_way_past_end = fl7::fonts::TextLayout::locate(codepoints, codepoints.size() + 1000, font, text_style, box_position, box_size);

    TESTLABS_CHECK_EQ_FLT( at_actual_end.position.x, at_way_past_end.position.x );
    TESTLABS_CHECK_EQ_FLT( at_actual_end.position.y, at_way_past_end.position.y );
}
