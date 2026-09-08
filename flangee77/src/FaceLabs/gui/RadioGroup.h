#ifndef FL7_GUI_RADIOGROUP_H
#define FL7_GUI_RADIOGROUP_H



namespace fl7::gui::faces {
class RadioButton;
} // namespace fl7::gui::faces

namespace fl7::gui {



/**
 * Ties together an arbitrary set of faces::RadioButton instances so that checking
 * one unchecks whichever other one (if any) was previously checked, deliberately
 * independent of the Face tree (unlike, e.g., some other toolkits' "same parent
 * container" grouping). Doesn't track its full membership, only whichever single
 * member is currently checked (if any): enough to enforce mutual exclusion without
 * needing to enumerate the group otherwise.
 */
class RadioGroup
{
    friend class faces::RadioButton;

public:
    RadioGroup() = default;

    RadioGroup(const RadioGroup&) = delete;
    RadioGroup& operator=(const RadioGroup&) = delete;
    RadioGroup(RadioGroup&&) = delete;
    RadioGroup& operator=(RadioGroup&&) = delete;

    ~RadioGroup() = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns the currently checked member of this group, or `nullptr` if none is. */
    faces::RadioButton* get_checked_button() const { return _checked_button; }



private:

    // #############################################################################
    // Backend Interface (RadioButton)
    // #############################################################################

    /**
     * Called by RadioButton::set_checked(true): remembers `button` as the newly
     * checked member, unchecking whichever other one was previously checked (if
     * any).
     */
    void _notify_checked(faces::RadioButton* button);

    /**
     * Forgets `button` as the checked member, if it currently is one. Called on an
     * explicit uncheck (RadioButton::set_checked(false)) and on a checked member's
     * destruction, so this group never keeps a dangling pointer or a stale "checked"
     * bookkeeping entry.
     */
    void _forget(faces::RadioButton* button);



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning. */
    faces::RadioButton* _checked_button = nullptr;

}; // class RadioGroup



} // namespace fl7::gui

#endif // FL7_GUI_RADIOGROUP_H
