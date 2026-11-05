#include "TextHelper.h"

#include "./AbstractRenderer.h"

#include "../Face.h"
#include "../Style.h"



namespace fl7::gui::render {



    /**
     * Draws text at absolute_position - text_scroll_offset, laid out against
     * text_box_size, styled per face's effective style, via renderer (see
     * HasText::get_text_scroll_offset()/get_text_box_size()).
     */
    void TextHelper::draw(AbstractRenderer* renderer, cl7::u32string_view text, const Face& face, ml7::Vector2f absolute_position, ml7::Vector2f text_scroll_offset, ml7::Vector2f text_box_size)
    {
        const Style& style = face.get_effective_style();
        renderer->draw_text_in_box(text, style.font, &style.text_style, absolute_position - text_scroll_offset, text_box_size);
    }



} // namespace fl7::gui::render
