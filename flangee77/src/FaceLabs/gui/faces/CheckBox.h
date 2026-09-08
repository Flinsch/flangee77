#ifndef FL7_GUI_FACES_CHECKBOX_H
#define FL7_GUI_FACES_CHECKBOX_H
#include "../Control.h"
#include "../HasBackground.h"
#include "../HasCheckedState.h"

#include <CoreLabs/behavioral/Signal.h>



namespace fl7::gui::faces {



/**
 * A togglable box with a persistent checked/unchecked value (see HasCheckedState).
 * Bare for now, no built-in caption: pair with a separate Label if one is wanted,
 * the way Panel/Frame's content areas already work with arbitrary child faces.
 */
class CheckBox
    : public Control
    , public HasBackground
    , public HasCheckedState
{

public:
    CheckBox() = default;

    explicit CheckBox(bool checked)
        : _checked(checked)
    {
    }

    CheckBox(const CheckBox&) = delete;
    CheckBox& operator=(const CheckBox&) = delete;
    CheckBox(CheckBox&&) = delete;
    CheckBox& operator=(CheckBox&&) = delete;

    ~CheckBox() override = default;



    // #############################################################################
    // HasCheckedState Implementations
    // #############################################################################

    /** Returns whether this checkbox is currently checked. */
    bool is_checked() const override { return _checked; }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets whether this checkbox is checked, unless already in that state. Emits
     * the "changed" signal (with the new value) when actually changed.
     */
    void set_checked(bool checked)
    {
        if (_checked == checked)
            return;

        _checked = checked;
        _changed.emit(_checked);
    }

    /** Returns the signal emitted (with the new value) whenever this checkbox's checked state actually changes. */
    cl7::behavioral::Signal<bool>& get_changed() { return _changed; }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this checkbox's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"checkbox"; }

    void _on_click(xl7::input::MouseButton button) override { set_checked(!_checked); }



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    bool _checked = false;

    cl7::behavioral::Signal<bool> _changed;

}; // class CheckBox



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_CHECKBOX_H
