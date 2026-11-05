#include "SelectionHelper.h"

#include "./AbstractRenderer.h"

#include "../Face.h"
#include "../HasSelection.h"
#include "../Style.h"

#include <FaceLabs/fonts/TextLayout.h>

#include <algorithm>
#include <span>



namespace fl7::gui::render {



    /**
     * Draws face's selection highlight (if any) against `text`, at
     * absolute_position, sized/styled per face's effective style, via renderer.
     * One rect per line the selection actually spans (see
     * fonts::TextLayout::lay_out_box()), rather than assuming there's only ever
     * the one, now that text may have grown to more than one line (see
     * gui::faces::TextField's multiline support).
     */
    void SelectionHelper::draw(AbstractRenderer* renderer, cl7::u32string_view text, const Face& face, ml7::Vector2f absolute_position)
    {
        const auto* has_selection = dynamic_cast<const HasSelection*>(&face);
        if (!has_selection || !has_selection->has_selection())
            return;

        const Style style = face.get_effective_style();
        if (!style.font)
            return;

        const size_t begin = std::min(has_selection->get_selection_begin_codepoint_index(), text.size());
        const size_t end = std::min(has_selection->get_selection_end_codepoint_index(), text.size());
        if (end <= begin)
            return;

        const std::vector<cl7::text::codec::codepoint> codepoints = fonts::TextLayout::decode(text);
        const fonts::TextBlockLayout block = fonts::TextLayout::lay_out_box(codepoints, *style.font, style.text_style, absolute_position, face.get_size());

        for (size_t i = 0; i < block.lines.size(); ++i)
        {
            const fonts::TextLine& line = block.lines[i];

            // The portion of [begin, end) that actually falls on this line.
            const size_t line_begin = std::max(begin, line.codepoint_begin);
            const size_t line_end = std::min(end, line.codepoint_end);
            if (line_end <= line_begin)
                continue;

            const float begin_x = block.left + fonts::TextLayout::measure_advance(std::span(codepoints).subspan(line.codepoint_begin, line_begin - line.codepoint_begin), *style.font, style.text_style);
            const float end_x = block.left + fonts::TextLayout::measure_advance(std::span(codepoints).subspan(line.codepoint_begin, line_end - line.codepoint_begin), *style.font, style.text_style);

            const float y = block.top + static_cast<float>(i) * block.line_height;
            renderer->draw_rect({begin_x, y}, {end_x, y + block.line_height}, style.selection_color);
        }
    }



} // namespace fl7::gui::render
