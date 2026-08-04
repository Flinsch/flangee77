#ifndef FL7_GUI_BUTTON_H
#define FL7_GUI_BUTTON_H
#include "./Control.h"

#include <CoreLabs/behavioral/Signal.h>
#include <CoreLabs/string.h>



namespace fl7::gui {



/**
 * A clickable piece of text-labeled chrome. Notifies interested code of a click and
 * other events via `get_clicked()` etc.
 */
class Button
    : public Control
{

public:
    Button() = default;

    explicit Button(cl7::u8string text)
        : _text(std::move(text))
    {
    }

    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    Button(Button&&) = delete;
    Button& operator=(Button&&) = delete;

    ~Button() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this button's text. */
    const cl7::u8string& get_text() const { return _text; }

    /** Sets this button's text. */
    void set_text(cl7::u8string text) { _text = std::move(text); }

    /** Returns the signal emitted whenever this button is clicked. */
    cl7::behavioral::Signal<>& get_clicked() { return _clicked; }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    void _on_click(xl7::input::MouseButton button) override { _clicked.emit(); }



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    cl7::u8string _text;

    cl7::behavioral::Signal<> _clicked;

}; // class Button



} // namespace fl7::gui

#endif // FL7_GUI_BUTTON_H
