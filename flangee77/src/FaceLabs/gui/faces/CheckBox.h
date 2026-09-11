#ifndef FL7_GUI_FACES_CHECKBOX_H
#define FL7_GUI_FACES_CHECKBOX_H
#include "../Compound.h"
#include "../HasCheckedState.h"

#include "./Label.h"

#include <CoreLabs/behavioral/Signal.h>



namespace fl7::gui::faces {



/**
 * A togglable box with a persistent checked/unchecked value (see HasCheckedState)
 * and an optional caption to its right. Internally a small fixed composition (see
 * Compound): a passive "box" glyph (see Box) sized to this face's own height and a
 * Label filling the remaining width for the caption. Both are disabled (see
 * Face::set_enabled), so a click or hover anywhere across box+caption always
 * resolves to this CheckBox as a whole, never to either part individually.
 */
class CheckBox
    : public Compound
    , public HasCheckedState
{

public:
    explicit CheckBox(bool checked = false, cl7::u32string text = {});

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
    void set_checked(bool checked);

    /** Returns the signal emitted (with the new value) whenever this checkbox's checked state actually changes. */
    cl7::behavioral::Signal<bool>& get_changed() { return _changed; }

    /** Returns this checkbox's caption text. */
    const cl7::u32string& get_text() const { return _label.get_text(); }

    /** Sets this checkbox's caption text. */
    void set_text(cl7::u32string text) { _label.set_text(std::move(text)); }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Keeps the box/caption layout in sync with this checkbox's own size. */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;

    /** Toggles the checked state. */
    void _on_click(xl7::input::MouseButton button) override { set_checked(!_checked); }



private:
    class Box; // Face + HasBackground only, defined in CheckBox.cpp; passive, see Box's own doc comment.

    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Box& _box;
    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Label& _label;

    bool _checked = false;

    cl7::behavioral::Signal<bool> _changed;

}; // class CheckBox



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_CHECKBOX_H
