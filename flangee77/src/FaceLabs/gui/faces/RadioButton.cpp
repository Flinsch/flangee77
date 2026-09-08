#include "RadioButton.h"

#include "../RadioGroup.h"



namespace fl7::gui::faces {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    RadioButton::RadioButton(RadioGroup* group, bool checked)
        : _group(group)
    {
        assert(_group);

        if (checked)
            set_checked(true);
    }

    RadioButton::~RadioButton()
    {
        _group->_forget(this);
    }



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
    void RadioButton::set_checked(bool checked)
    {
        if (_checked == checked)
            return;

        _checked = checked;

        if (_checked)
            _group->_notify_checked(this);
        else
            _group->_forget(this);

        _changed.emit(_checked);
    }



    // #############################################################################
    // Backend Interface (RadioGroup)
    // #############################################################################

    /**
     * Unchecks this radio button directly, without notifying _group (called by
     * RadioGroup itself, once it already knows about the newly checked member, to
     * avoid recursing back into it).
     */
    void RadioButton::_force_unchecked()
    {
        if (!_checked)
            return;

        _checked = false;
        _changed.emit(false);
    }



} // namespace fl7::gui::faces
