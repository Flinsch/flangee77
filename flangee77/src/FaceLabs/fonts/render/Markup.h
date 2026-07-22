#ifndef FL7_FONTS_RENDER_MARKUP_H
#define FL7_FONTS_RENDER_MARKUP_H

#include "./StyleRun.h"

#include "../IconRun.h"
#include "../TextStyle.h"

#include <CoreLabs/string.h>
#include <CoreLabs/text/codec/codepoint_iterator.h>

#include <span>
#include <vector>



namespace fl7::fonts::render {



/**
 * An icon usable from markup via `[icon=name]`, resolved by exact name match.
 */
struct NamedIcon
{
    cl7::u8string_view name;
    const Icon* icon = nullptr;
};

/**
 * The result of parsing markup text: ready to hand straight to
 * AbstractRenderer::draw_text/draw_text_in_box.
 */
struct ParsedMarkup
{
    cl7::u8string text;
    std::vector<StyleRun> style_runs;
    std::vector<IconRun> icon_runs;
};



    /**
     * Parses a small, closed set of inline markup tags out of `text`:
     *
     * - [b]bold[/b]                    GlyphStyle::weight = 1 (or `[b=1.5]` for a custom value)
     * - [i]italic[/i]                  GlyphStyle::italic_intensity = 1 (or `[i=0.5]`)
     * - [color=#RRGGBB]text[/color]    GlyphStyle::text_color (also accepts #RRGGBBAA)
     * - [outline=#RRGGBB]text[/outline] GlyphStyle::outline_color, with a fixed default
     *                                  outline_width (also accepts #RRGGBBAA; no separate
     *                                  width attribute yet)
     * - [icon=name]                    an inline Icon, looked up by exact name in `icons` (no closing tag)
     *
     * Tags may be nested to arbitrary depth; the effective style at any point is
     * the merge of all currently-open tags on top of `base_style`'s (inherited)
     * GlyphStyle, e.g., `[b][color=#ff0000]bold red[/color] just bold[/b]` works as
     * expected. `base_style` is only used for this seeding, it is not otherwise
     * modified or consulted.
     *
     * The grammar is deliberately strict and closed: anything that doesn't exactly
     * match one of the tags above: an unknown tag name, a malformed/missing
     * attribute, a stray or mismatched closing tag, or an unresolved icon name,
     * these are left in the output as literal text, unchanged (including their
     * brackets). This means arbitrary game-UI text containing bracketed key hints
     * (e.g., "[E] Interact") passes through untouched, with no escaping needed.
     *
     * Icon positions in the returned (stripped) text use U+FFFC (OBJECT REPLACEMENT
     * CHARACTER) as their placeholder code point.
     */
    template <cl7::any_string_view_like Tstring_view_like>
    ParsedMarkup parse_markup(Tstring_view_like&& text, const TextStyle& base_style, std::span<const NamedIcon> icons = {});

    /** The actual (non-template) parsing implementation, see parse_markup above. */
    ParsedMarkup _parse_markup(std::span<const cl7::text::codec::codepoint> codepoints, const TextStyle& base_style, std::span<const NamedIcon> icons);

    template <cl7::any_string_view_like Tstring_view_like>
    ParsedMarkup parse_markup(Tstring_view_like&& text, const TextStyle& base_style, std::span<const NamedIcon> icons)
    {
        auto sv = cl7::make_string_view(std::forward<Tstring_view_like>(text));
        using codepoint_iterator = cl7::text::codec::codepoint_iterator<cl7::char_type_of_t<Tstring_view_like>>;

        std::vector<cl7::text::codec::codepoint> codepoints;
        for (codepoint_iterator it(sv); it != codepoint_iterator(); ++it)
            codepoints.push_back(*it);

        return _parse_markup(codepoints, base_style, icons);
    }



} // namespace fl7::fonts::render

#endif // FL7_FONTS_RENDER_MARKUP_H
