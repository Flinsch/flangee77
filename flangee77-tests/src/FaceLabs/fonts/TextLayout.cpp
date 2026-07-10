
#include <TestLabs/TestSuite.h>

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
