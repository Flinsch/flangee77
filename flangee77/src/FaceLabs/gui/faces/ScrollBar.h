#ifndef FL7_GUI_FACES_SCROLLBAR_H
#define FL7_GUI_FACES_SCROLLBAR_H
#include "../Compound.h"
#include "../TrackControl.h"

#include "./Button.h"
#include "./Slider.h"

#include <CoreLabs/behavioral/Signal.h>



namespace fl7::gui::faces {



/**
 * A Compound of a Slider (see its own get_handle_fraction()) between two
 * optional step buttons (get_show_buttons()), like any other GUI's scroll bar:
 * the slider's handle represents get_visible_fraction() of some larger
 * content -- outside this face's own knowledge, e.g. a Panel's content vs. its
 * visible area -- that's currently visible, and the buttons nudge the value
 * by get_step() per click, same as a Left/Right/Up/Down key press (or the
 * mouse wheel) already would against the slider itself. This face has no
 * visual role of its own (no background, no theme key): it's purely a layout
 * container, its slider/buttons carry their own themed roles.
 */
class ScrollBar
    : public Compound
{

public:
    using Orientation = TrackControl::Orientation;

    explicit ScrollBar(Orientation orientation = Orientation::Vertical, float visible_fraction = 1.0f, float value = 0.0f, float min = 0.0f, float max = 1.0f, bool show_buttons = true);

    ScrollBar(const ScrollBar&) = delete;
    ScrollBar& operator=(const ScrollBar&) = delete;
    ScrollBar(ScrollBar&&) = delete;
    ScrollBar& operator=(ScrollBar&&) = delete;

    ~ScrollBar() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this scroll bar's orientation. */
    Orientation get_orientation() const { return _orientation; }

    /** Returns this scroll bar's current value. */
    float get_value() const { return _slider.get_value(); }

    /** Sets this scroll bar's value, clamped to [get_min(), get_max()]. */
    void set_value(float value) { _slider.set_value(value); }

    /** Returns this scroll bar's minimum value. */
    float get_min() const { return _slider.get_min(); }

    /** Returns this scroll bar's maximum value. */
    float get_max() const { return _slider.get_max(); }

    /** Returns the amount a step button (or a key press, or a wheel notch) nudges the value by. */
    float get_step() const { return _slider.get_step(); }

    /** Sets the amount a step button (or a key press, or a wheel notch) nudges the value by. */
    void set_step(float step) { _slider.set_step(step); }

    /** Returns how much of the larger content this scroll bar represents is currently visible, in (0, 1]. */
    float get_visible_fraction() const { return *_slider.get_handle_fraction(); }

    /**
     * Sets how much of the larger content this scroll bar represents is currently
     * visible, clamped to (0, 1].
     */
    void set_visible_fraction(float visible_fraction);

    /** Returns whether the two step buttons are currently shown. */
    bool get_show_buttons() const { return _show_buttons; }

    /**
     * Sets whether the two step buttons are shown at all; hidden, the slider fills
     * the whole length instead.
     */
    void set_show_buttons(bool show_buttons);

    /** Returns the signal emitted (with the new value) whenever this scroll bar's value actually changes. */
    cl7::behavioral::Signal<float>& get_changed() { return _slider.get_changed(); }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Keeps the slider/buttons' sizes/positions in sync with this scroll bar's own size. */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Lays out the step buttons (if shown) at each end and the slider filling
     * whatever's left between them.
     */
    void _layout();



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Slider& _slider;
    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Button& _decrement_button;
    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Button& _increment_button;

    Orientation _orientation;
    bool _show_buttons;

}; // class ScrollBar



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_SCROLLBAR_H
