#ifndef FL7_GUI_CONTROL_H
#define FL7_GUI_CONTROL_H
#include "./Face.h"



namespace fl7::gui {



/**
 * Taxonomic base for interactive leaf faces that respond to input (Button,
 * CheckBox, TextField, RadioButton, Slider). Purely a marker for now -- no
 * new members; concrete subclasses meaningfully override Face's input-event
 * hooks (_on_click() etc.), which is what actually distinguishes a Control
 * from a Static in practice.
 */
class Control
    : public Face
{

public:
    Control() = default;

    Control(const Control&) = delete;
    Control& operator=(const Control&) = delete;
    Control(Control&&) = delete;
    Control& operator=(Control&&) = delete;

    ~Control() override = default;



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Every Control is a Tab stop by definition: that's what distinguishes it from a Static. */
    bool _is_focusable() const override { return true; }

}; // class Control



} // namespace fl7::gui

#endif // FL7_GUI_CONTROL_H
