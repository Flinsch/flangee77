#ifndef FL7_GUI_FACES_SLIDER_H
#define FL7_GUI_FACES_SLIDER_H
#include "../Compound.h"
#include "../HasBackground.h"

#include <CoreLabs/behavioral/Signal.h>



namespace fl7::gui::faces {



/**
 * A horizontal track with a draggable handle representing a value in
 * [get_min(), get_max()]. Internally a small fixed composition (see Compound):
 * this face itself is the track (see HasBackground, themed "slider"), and a
 * passive "handle" glyph (see Handle, themed "slider_handle") is positioned
 * along it according to the current value. Click anywhere on the track to jump
 * the handle there, or drag it (from the handle or the track, doesn't matter);
 * while focused, Left/Down/Right/Up nudge the value by get_step(), Home/End
 * jump to the min/max.
 */
class Slider
    : public Compound
    , public HasBackground
{

public:
    explicit Slider(float value = 0.0f, float min = 0.0f, float max = 1.0f);

    Slider(const Slider&) = delete;
    Slider& operator=(const Slider&) = delete;
    Slider(Slider&&) = delete;
    Slider& operator=(Slider&&) = delete;

    ~Slider() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this slider's current value. */
    float get_value() const { return _value; }

    /**
     * Sets this slider's value, clamped to [get_min(), get_max()], unless
     * already at that (clamped) value. Emits the "changed" signal (with the
     * new value) when actually changed.
     */
    void set_value(float value);

    /** Returns this slider's minimum value. */
    float get_min() const { return _min; }

    /** Returns this slider's maximum value. */
    float get_max() const { return _max; }

    /** Returns the amount Left/Right/Up/Down nudge the value by. */
    float get_step() const { return _step; }

    /** Sets the amount Left/Right/Up/Down nudge the value by. */
    void set_step(float step) { _step = step; }

    /** Returns the signal emitted (with the new value) whenever this slider's value actually changes. */
    cl7::behavioral::Signal<float>& get_changed() { return _changed; }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this slider's (track's) theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"slider"; }

    /** Keeps the handle's size/position in sync with this slider's own size. */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;

    /** Jumps the handle to the clicked position. */
    void _on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position) override;

    /** Drags the handle to wherever the cursor now is. */
    void _on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position) override;

    /** Left/Down/Right/Up nudge the value by get_step(); Home/End jump to the min/max. */
    void _on_key_down(xl7::input::Key key) override;



private:
    class Handle; // Face + HasBackground only, defined in Slider.cpp; passive, see Handle's own doc comment.

    // #############################################################################
    // Helpers
    // #############################################################################

    /** Returns the value the handle would represent if its center were at local_x. */
    float _value_at(float local_x) const;

    /** Sets the value the handle would represent if its center were at local_x. */
    void _set_value_from_local_x(float local_x);

    /** Resizes/repositions the handle for the current size/value. */
    void _update_handle();



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Handle& _handle;

    float _min;
    float _max;
    float _value;

    /** The amount Left/Right/Up/Down nudge the value by. */
    float _step;

    cl7::behavioral::Signal<float> _changed;

}; // class Slider



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_SLIDER_H
