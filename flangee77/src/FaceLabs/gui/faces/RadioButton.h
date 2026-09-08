#ifndef FL7_GUI_FACES_RADIOBUTTON_H
#define FL7_GUI_FACES_RADIOBUTTON_H
#include "../Control.h"
#include "../HasBackground.h"
#include "../HasCheckedState.h"

#include <CoreLabs/behavioral/Signal.h>



namespace fl7::gui {
class RadioGroup;
} // namespace fl7::gui

namespace fl7::gui::faces {



/**
 * A togglable box with a persistent checked/unchecked value (see HasCheckedState),
 * mutually exclusive with the other members of its RadioGroup: checking this one
 * unchecks whichever other member of the same group was previously checked. Bare
 * for now, no built-in caption, same as CheckBox.
 */
class RadioButton
    : public Control
    , public HasBackground
    , public HasCheckedState
{
    friend class fl7::gui::RadioGroup;

public:
    explicit RadioButton(RadioGroup* group, bool checked = false);

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



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this radio button's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"radio_button"; }

    /** Always checks (never toggles off): clicking an already-checked radio button does nothing. */
    void _on_click(xl7::input::MouseButton button) override { set_checked(true); }



private:

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

    /** Non-owning: whoever builds the group hierarchy keeps it alive. */
    RadioGroup* _group;

    bool _checked = false;

    cl7::behavioral::Signal<bool> _changed;

}; // class RadioButton



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_RADIOBUTTON_H
