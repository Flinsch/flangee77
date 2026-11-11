#ifndef FL7_GUI_FACES_BUTTON_H
#define FL7_GUI_FACES_BUTTON_H
#include "../Control.h"
#include "../HasBackground.h"
#include "../HasText.h"

#include <FaceLabs/fonts/Font.h>

#include <CoreLabs/behavioral/Signal.h>
#include <CoreLabs/string.h>



namespace fl7::gui::faces {



/**
 * A clickable piece of text-labeled chrome. Notifies interested code of a click and
 * other events via `get_clicked()` etc.
 */
class Button
    : public Control
    , public HasBackground
    , public HasText
{

public:
    Button() = default;

    explicit Button(cl7::u32string text)
        : _text(std::move(text))
    {
    }

    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    Button(Button&&) = delete;
    Button& operator=(Button&&) = delete;

    ~Button() override = default;



    // #############################################################################
    // HasText Implementations
    // #############################################################################

    /** Returns this button's text to be displayed. */
    const cl7::u32string& get_display_text() const override { return _text; }



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this button's text. */
    const cl7::u32string& get_text() const { return _text; }

    /** Sets this button's text. */
    void set_text(cl7::u32string text) { _text = std::move(text); }

    /** Returns the signal emitted whenever this button is clicked. */
    cl7::behavioral::Signal<>& get_clicked() { return _clicked; }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this button's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"button"; }

    void _on_click(xl7::input::MouseButton button) override { _clicked.emit(); }

    /** A Button's _on_click() meaningfully does something, so Enter/Space (or a gamepad's "confirm" button) can activate it while focused. */
    bool _is_clickable() const override { return true; }

    /**
     * Returns this button's text's own natural (unwrapped) size, under its
     * effective font/text style; see _calculate_text_size().
     */
    ml7::Vector2f _calculate_preferred_size() const override;

    /**
     * Returns this button's text's own natural (unwrapped) size, under its
     * effective font/text style; see _calculate_text_size().
     */
    ml7::Vector2f _calculate_minimum_size() const override;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns this button's text's own natural (unwrapped) size: however wide its
     * single widest (explicit-break-delimited) line is, times however many such
     * lines there are, under its effective font/text style. {0, 0} if it currently
     * has no effective font (e.g., not yet part of a themed Shell). Cached, as
     * long as neither the text nor the effective font have changed since.
     */
    ml7::Vector2f _calculate_text_size() const;



    // #############################################################################
    // Attributes
    // #############################################################################

    cl7::u32string _text;

    cl7::behavioral::Signal<> _clicked;

    /** The inputs/result of the last _calculate_text_size() call; see there. */
    mutable fonts::Font* _cached_font = nullptr;
    mutable cl7::u32string _cached_text;
    mutable ml7::Vector2f _cached_text_size;

}; // class Button



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_BUTTON_H
