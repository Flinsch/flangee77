#include "CaretHelper.h"

#include "./AbstractRenderer.h"

#include "../Face.h"
#include "../HasCaret.h"
#include "../Style.h"

#include <FaceLabs/fonts/TextLayout.h>

#include <algorithm>



namespace fl7::gui::render {



    /**
     * Draws face's caret (if any, and currently visible) against `text`, at
     * absolute_position, sized/styled per face's effective style, via renderer.
     */
    void CaretHelper::draw(AbstractRenderer* renderer, cl7::u32string_view text, const Face& face, ml7::Vector2f absolute_position)
    {
        const auto* has_caret = dynamic_cast<const HasCaret*>(&face);
        if (!has_caret || !has_caret->is_caret_visible())
            return;

        const Style style = face.get_effective_style();
        if (!style.font)
            return;

        const size_t caret_index = std::min(has_caret->get_caret_codepoint_index(), text.size());
        const float caret_x = fonts::TextLayout::measure_advance(text.substr(0, caret_index), *style.font, style.text_style);

        constexpr float caret_width = 1.5f;
        constexpr float vertical_margin = 2.0f;

        const ml7::Vector2f caret_min = absolute_position + ml7::Vector2f{style.text_style.background_padding.x + caret_x, vertical_margin};
        const ml7::Vector2f caret_max = caret_min + ml7::Vector2f{caret_width, std::max(0.0f, face.get_size().y - 2.0f * vertical_margin)};

        renderer->draw_rect(caret_min, caret_max, style.text_style.text_color);
    }



} // namespace fl7::gui::render
