#ifndef FL7_GUI_HASCHECKEDSTATE_H
#define FL7_GUI_HASCHECKEDSTATE_H



namespace fl7::gui {



/**
 * Interface: opts a face into having a persistent, semantic "checked" value
 * (CheckBox, RadioButton, ...), as opposed to the transient, Shell-tracked
 * interaction states (hovered/pressed/focused, see Theme::State) that apply
 * to any face uniformly. Queried by Face::get_effective_style() to resolve
 * the "checked" theme overlay (see ThemeLevel), applied before those.
 */
class HasCheckedState
{

public:
    virtual ~HasCheckedState() = default;

    /**
     * Returns whether this face is currently checked.
     */
    virtual bool is_checked() const = 0;

}; // class HasCheckedState



} // namespace fl7::gui

#endif // FL7_GUI_HASCHECKEDSTATE_H
