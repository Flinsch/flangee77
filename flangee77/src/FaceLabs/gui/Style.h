#ifndef FL7_GUI_STYLE_H
#define FL7_GUI_STYLE_H

#include <FaceLabs/fonts/Font.h>
#include <FaceLabs/fonts/TextStyle.h>

#include <XiaoLabs/graphics/Color.h>



namespace fl7::gui {



/**
 * The visual properties a Face draws itself with.
 */
struct Style
{

    /** Non-owning: whoever builds the Style keeps the Font alive. */
    fonts::Font* font = nullptr;

    fonts::TextStyle text_style;

    xl7::graphics::Color background_color = {0.2f, 0.2f, 0.2f, 1.0f};
    xl7::graphics::Color border_color = {0.0f, 0.0f, 0.0f, 1.0f};
    /** 0 = no border. */
    float border_width = 0.0f;

}; // struct Style



} // namespace fl7::gui

#endif // FL7_GUI_STYLE_H
