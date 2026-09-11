#ifndef FL7_GUI_FACES_RADIOBUTTON_H
#define FL7_GUI_FACES_RADIOBUTTON_H
#include "../Compound.h"
#include "../HasCheckedState.h"

#include "./Label.h"

#include <CoreLabs/behavioral/Signal.h>



namespace fl7::gui {
class RadioGroup;
} // namespace fl7::gui

namespace fl7::gui::faces {



/**
 * A togglable box with a persistent checked/unchecked value (see HasCheckedState)
 * and an optional caption to its right, mutually exclusive with the other members
 * of its RadioGroup: checking this one unchecks whichever other member of the same
 * group was previously checked. Composed internally the same way as CheckBox (see
 * there for the rationale): a passive "box" glyph (see Box) plus a Label caption,
 * both disabled so clicks/hover across either part resolve to this RadioButton as a
 * whole.
 */
class RadioButton
    : public Compound
    , public HasCheckedState
{
    friend class fl7::gui::RadioGroup;

public:
    explicit RadioButton(RadioGroup* group, bool checked = false, cl7::u32string text = {});

    RadioButton(const RadioButton&) = delete;
    RadioButton& operator=(const RadioButton&) = delete;
    RadioButton(RadioButton&&) = delete;
    RadioButton& operator=(RadioButton&&) = delete;

    ~RadioButton() override;



    // #############################################################################
    // HasCheckedState Implementations
    // #############################################################################

    /** Returns whether this radio button is currently checked. */
    bool is_checked() const override { return _checked; }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets whether this radio button is checked, unless already in that state.
     * Checking it unchecks whichever other member of its group was previously
     * checked. Explicitly unchecking it (unlike a direct click, see _on_click())
     * is still allowed, e.g. to start a group out with nothing selected. Emits
     * the "changed" signal (with the new value) when actually changed.
     */
    void set_checked(bool checked);

    /** Returns the signal emitted (with the new value) whenever this radio button's checked state actually changes. */
    cl7::behavioral::Signal<bool>& get_changed() { return _changed; }

    /** Returns this radio button's caption text. */
    const cl7::u32string& get_text() const { return _label.get_text(); }

    /** Sets this radio button's caption text. */
    void set_text(cl7::u32string text) { _label.set_text(std::move(text)); }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Keeps the box/caption layout in sync with this radio button's own size. */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;

    /** Always checks (never toggles off): clicking an already-checked radio button does nothing. */
    void _on_click(xl7::input::MouseButton button) override { set_checked(true); }



private:
    class Box; // Face + HasBackground only, defined in RadioButton.cpp; passive, see Box's own doc comment.

    // #############################################################################
    // Backend Interface (RadioGroup)
    // #############################################################################

    /**
     * Unchecks this radio button directly, without notifying _group (called by
     * RadioGroup itself, once it already knows about the newly checked member, to
     * avoid recursing back into it).
     */
    void _force_unchecked();



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Box& _box;
    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Label& _label;

    /** Non-owning: whoever builds the group hierarchy keeps it alive. */
    RadioGroup* _group;

    bool _checked = false;

    cl7::behavioral::Signal<bool> _changed;

}; // class RadioButton



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_RADIOBUTTON_H
