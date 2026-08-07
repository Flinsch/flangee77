#include "TextHelper.h"

#include "./AbstractRenderer.h"

#include "../Face.h"
#include "../Style.h"



namespace fl7::gui::render {



    /**
     * Draws text at absolute_position, sized/styled per face's effective style, via
     * renderer.
     */
    void TextHelper::draw(AbstractRenderer* renderer, cl7::u8string_view text, const Face& face, ml7::Vector2f absolute_position)
    {
        const Style& style = face.get_effective_style();
        renderer->draw_text_in_box(text, style.font, &style.text_style, absolute_position, face.get_size());
    }



} // namespace fl7::gui::render
