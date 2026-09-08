#include "RadioGroup.h"

#include "./faces/RadioButton.h"



namespace fl7::gui {



    // #############################################################################
    // Backend Interface (RadioButton)
    // #############################################################################

    /**
     * Called by RadioButton::set_checked(true): remembers `button` as the newly
     * checked member, unchecking whichever other one was previously checked (if
     * any).
     */
    void RadioGroup::_notify_checked(faces::RadioButton* button)
    {
        if (_checked_button == button)
            return;

        faces::RadioButton* previous = _checked_button;
        _checked_button = button;

        if (previous)
            previous->_force_unchecked();
    }

    /**
     * Forgets `button` as the checked member, if it currently is one. Called on an
     * explicit uncheck (RadioButton::set_checked(false)) and on a checked member's
     * destruction, so this group never keeps a dangling pointer or a stale "checked"
     * bookkeeping entry.
     */
    void RadioGroup::_forget(faces::RadioButton* button)
    {
        if (_checked_button == button)
            _checked_button = nullptr;
    }



} // namespace fl7::gui
