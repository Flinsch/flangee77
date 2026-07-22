
#include <TestLabs/TestSuite.h>

#include <FaceLabs/fonts/render/Markup.h>

#include "../../../shared.h"

#include <CoreLabs/text/codec/codepoint_iterator.h>

#include <vector>



namespace {

    std::vector<cl7::text::codec::codepoint> to_codepoints(cl7::u8string_view s)
    {
        std::vector<cl7::text::codec::codepoint> codepoints;
        for (cl7::text::codec::codepoint_iterator<cl7::u8char_t> it(s); it != decltype(it)(); ++it)
            codepoints.push_back(*it);
        return codepoints;
    }

} // namespace



TESTLABS_CASE( u8"FaceLabs:  fonts:  render:  Markup:  parse_markup - plain text without tags is unchanged" )
{
    fl7::fonts::TextStyle base_style;

    const auto parsed = fl7::fonts::render::parse_markup(u8"Hello, World!", base_style);

    TESTLABS_CHECK_EQ( parsed.text, u8"Hello, World!" );
    TESTLABS_CHECK( parsed.style_runs.empty() );
    TESTLABS_CHECK( parsed.icon_runs.empty() );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  render:  Markup:  parse_markup - [b]...[/b] produces one StyleRun, base color preserved" )
{
    fl7::fonts::TextStyle base_style;
    base_style.text_color = xl7::graphics::Color::CYAN;

    const auto parsed = fl7::fonts::render::parse_markup(u8"a[b]bold[/b]b", base_style);

    TESTLABS_CHECK_EQ( parsed.text, u8"aboldb" );
    TESTLABS_CHECK_EQ( parsed.style_runs.size(), size_t{1} );
    const auto& run = parsed.style_runs.at(0);
    TESTLABS_CHECK_EQ( run.codepoint_begin, size_t{1} );
    TESTLABS_CHECK_EQ( run.codepoint_end, size_t{5} );
    TESTLABS_CHECK_EQ_FLT( run.style.weight, 1.0f );
    TESTLABS_CHECK( run.style.text_color == xl7::graphics::Color::CYAN );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  render:  Markup:  parse_markup - nested tags merge into combined StyleRuns" )
{
    fl7::fonts::TextStyle base_style;

    const auto parsed = fl7::fonts::render::parse_markup(u8"[b]bold[color=#ff0000]bold red[/color]bold again[/b]", base_style);

    TESTLABS_CHECK_EQ( parsed.text, u8"boldbold redbold again" );
    TESTLABS_CHECK_EQ( parsed.style_runs.size(), size_t{3} );

    // "bold" (weight only)
    TESTLABS_CHECK_EQ_FLT( parsed.style_runs.at(0).style.weight, 1.0f );
    TESTLABS_CHECK( parsed.style_runs.at(0).style.text_color == base_style.text_color );

    // "bold red" (weight + color)
    TESTLABS_CHECK_EQ_FLT( parsed.style_runs.at(1).style.weight, 1.0f );
    TESTLABS_CHECK( parsed.style_runs.at(1).style.text_color == xl7::graphics::Color::RED );

    // "bold again" (back to weight only)
    TESTLABS_CHECK_EQ_FLT( parsed.style_runs.at(2).style.weight, 1.0f );
    TESTLABS_CHECK( parsed.style_runs.at(2).style.text_color == base_style.text_color );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  render:  Markup:  parse_markup - [outline=#RRGGBB]...[/outline] sets outline_color and a default outline_width" )
{
    fl7::fonts::TextStyle base_style;

    const auto parsed = fl7::fonts::render::parse_markup(u8"a[outline=#000000]outlined[/outline]b", base_style);

    TESTLABS_CHECK_EQ( parsed.text, u8"aoutlinedb" );
    TESTLABS_CHECK_EQ( parsed.style_runs.size(), size_t{1} );
    const auto& run = parsed.style_runs.at(0);
    TESTLABS_CHECK( run.style.outline_color == xl7::graphics::Color::BLACK );
    TESTLABS_CHECK_GT( run.style.outline_width, 0.0f );
    // Unrelated fields stay at the base style, same as [b]/[color] leave each other alone.
    TESTLABS_CHECK_EQ_FLT( run.style.weight, base_style.weight );
    TESTLABS_CHECK( run.style.text_color == base_style.text_color );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  render:  Markup:  parse_markup - [outline] nests with [b]/[color] like any other tag" )
{
    fl7::fonts::TextStyle base_style;

    const auto parsed = fl7::fonts::render::parse_markup(u8"[b][outline=#000000]bold outlined[/outline][/b]", base_style);

    TESTLABS_CHECK_EQ( parsed.text, u8"bold outlined" );
    TESTLABS_CHECK_EQ( parsed.style_runs.size(), size_t{1} );
    const auto& run = parsed.style_runs.at(0);
    TESTLABS_CHECK_EQ_FLT( run.style.weight, 1.0f );
    TESTLABS_CHECK( run.style.outline_color == xl7::graphics::Color::BLACK );
    TESTLABS_CHECK_GT( run.style.outline_width, 0.0f );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  render:  Markup:  parse_markup - unrecognized/stray brackets fall back to literal text" )
{
    fl7::fonts::TextStyle base_style;

    const auto unknown_tag = fl7::fonts::render::parse_markup(u8"Press [Space] to jump", base_style);
    TESTLABS_CHECK_EQ( unknown_tag.text, u8"Press [Space] to jump" );
    TESTLABS_CHECK( unknown_tag.style_runs.empty() );

    const auto stray_close = fl7::fonts::render::parse_markup(u8"no open tag[/b] here", base_style);
    TESTLABS_CHECK_EQ( stray_close.text, u8"no open tag[/b] here" );
    TESTLABS_CHECK( stray_close.style_runs.empty() );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  render:  Markup:  parse_markup - an unclosed tag extends to the end of the text" )
{
    fl7::fonts::TextStyle base_style;

    const auto parsed = fl7::fonts::render::parse_markup(u8"a[b]bold to the end", base_style);

    TESTLABS_CHECK_EQ( parsed.text, u8"abold to the end" );
    TESTLABS_CHECK_EQ( parsed.style_runs.size(), size_t{1} );
    TESTLABS_CHECK_EQ( parsed.style_runs.at(0).codepoint_begin, size_t{1} );
    TESTLABS_CHECK_EQ( parsed.style_runs.at(0).codepoint_end, parsed.text.size() );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  render:  Markup:  parse_markup - [icon=name] resolves against the icon registry" )
{
    fl7::fonts::TextStyle base_style;
    const fl7::fonts::Icon gem_icon{.size = {24.0f, 24.0f}};
    const fl7::fonts::render::NamedIcon icons[] = {
        {.name = u8"gem", .icon = &gem_icon},
    };

    const auto parsed = fl7::fonts::render::parse_markup(u8"a[icon=gem]b", base_style, icons);

    const auto codepoints = to_codepoints(parsed.text);
    TESTLABS_CHECK_EQ( codepoints.size(), size_t{3} );
    TESTLABS_CHECK_EQ( codepoints.at(1).value, cl7::text::codec::codepoint::value_type{0xFFFC} );
    TESTLABS_CHECK_EQ( parsed.icon_runs.size(), size_t{1} );
    TESTLABS_CHECK_EQ( parsed.icon_runs.at(0).codepoint_index, size_t{1} );
    TESTLABS_CHECK_EQ( parsed.icon_runs.at(0).icon, &gem_icon );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  render:  Markup:  parse_markup - an unresolved icon name falls back to literal text" )
{
    fl7::fonts::TextStyle base_style;

    const auto parsed = fl7::fonts::render::parse_markup(u8"a[icon=unknown]b", base_style);

    TESTLABS_CHECK_EQ( parsed.text, u8"a[icon=unknown]b" );
    TESTLABS_CHECK( parsed.icon_runs.empty() );
}
