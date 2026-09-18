#include "SelectionHelper.h"

#include "./AbstractRenderer.h"

#include "../Face.h"
#include "../HasSelection.h"
#include "../Style.h"

#include <FaceLabs/fonts/TextLayout.h>

#include <algorithm>



namespace fl7::gui::render {



    /**
     * Draws face's selection highlight (if any) against `text`, at
     * absolute_position, sized/styled per face's effective style, via renderer.
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

        const float begin_x = fonts::TextLayout::measure_advance(text.substr(0, begin), *style.font, style.text_style);
        const float end_x = fonts::TextLayout::measure_advance(text.substr(0, end), *style.font, style.text_style);

        const ml7::Vector2f offset{style.text_style.background_padding.x, 0.0f};
        const ml7::Vector2f highlight_min = absolute_position + offset + ml7::Vector2f{begin_x, 0.0f};
        const ml7::Vector2f highlight_max = absolute_position + offset + ml7::Vector2f{end_x, face.get_size().y};

        renderer->draw_rect(highlight_min, highlight_max, style.selection_color);
    }



} // namespace fl7::gui::render
