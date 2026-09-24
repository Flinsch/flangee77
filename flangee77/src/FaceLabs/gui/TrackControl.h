#ifndef FL7_GUI_TRACKCONTROL_H
#define FL7_GUI_TRACKCONTROL_H
#include "./Compound.h"
#include "./HasBackground.h"

#include <CoreLabs/behavioral/Signal.h>



namespace fl7::gui {



/**
 * Shared base for a track with a draggable handle representing a value in
 * [get_min(), get_max()] (see Slider, ScrollBar): owns the value/step model, the
 * click-to-jump/drag-to-follow/keyboard-nudge interaction, and the handle's
 * size/position along the track, all generically over Orientation. This face itself
 * is the track (see HasBackground): a concrete subclass supplies its own themed
 * "handle" child (see _get_handle()) and, if its handle isn't a fixed square (the
 * default, see _get_handle_main_size()), how big that handle actually is (e.g.,
 * ScrollBar: proportional to how much of some larger content is currently visible).
 */
class TrackControl
    : public Compound
    , public HasBackground
{

public:
    enum struct Orientation
    {
        Horizontal,
        Vertical,
    };



    TrackControl(const TrackControl&) = delete;
    TrackControl& operator=(const TrackControl&) = delete;
    TrackControl(TrackControl&&) = delete;
    TrackControl& operator=(TrackControl&&) = delete;

    ~TrackControl() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this track's orientation. */
    Orientation get_orientation() const { return _orientation; }

    /** Returns this track's current value. */
    float get_value() const { return _value; }

    /**
     * Sets this track's value, clamped to [get_min(), get_max()], unless already at
     * that (clamped) value. Emits the "changed" signal (with the new value) when
     * actually changed.
     */
    void set_value(float value);

    /** Returns this track's minimum value. */
    float get_min() const { return _min; }

    /** Returns this track's maximum value. */
    float get_max() const { return _max; }

    /** Returns the amount Left/Right/Up/Down nudge the value by. */
    float get_step() const { return _step; }

    /** Sets the amount Left/Right/Up/Down nudge the value by. */
    void set_step(float step) { _step = step; }

    /** Returns the signal emitted (with the new value) whenever this track's value actually changes. */
    cl7::behavioral::Signal<float>& get_changed() { return _changed; }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Constructs a track with the given orientation, initial (clamped) value, and
     * min/max. A subclass must still add its own handle child (via the inherited
     * add_child(), same as any other Compound) and expose it via _get_handle()
     * before this face is actually sized/used. See Slider's constructor for the
     * pattern, for example.
     */
    TrackControl(Orientation orientation, float value, float min, float max);



    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Returns this track's handle child, for the shared logic below to size/
     * position it against. A plain Face& (not, say, a HasBackground&): this class
     * only ever moves/resizes it, drawing is entirely up to whatever the concrete
     * handle type itself opts into (see Slider::Handle).
     */
    virtual Face& _get_handle() = 0;

    /**
     * Returns the handle's size along the main axis (the track's own length).
     * Default: the track's own cross-axis size, i.e., a fixed square handle
     * (Slider's look). Override for a handle whose length is itself meaningful,
     * e.g., ScrollBar: proportional to how much of some larger content is currently
     * visible.
     */
    virtual float _get_handle_main_size() const;



    // #############################################################################
    // Helpers
    // #############################################################################

    /** Returns size's component along this track's main axis (x if Horizontal, y if Vertical). */
    float _main(ml7::Vector2f size) const { return _orientation == Orientation::Horizontal ? size.x : size.y; }

    /** Returns size's component along this track's cross axis (the opposite of _main()). */
    float _cross(ml7::Vector2f size) const { return _orientation == Orientation::Horizontal ? size.y : size.x; }

    /** Builds a Vector2f from a main-axis and a cross-axis component, placed according to this track's orientation. */
    ml7::Vector2f _make_vector(float main, float cross) const { return _orientation == Orientation::Horizontal ? ml7::Vector2f{main, cross} : ml7::Vector2f{cross, main}; }

    /**
     * Resizes/repositions the handle for the current size/value. Also needed
     * whenever a subclass's own _get_handle_main_size() input changes (see
     * ScrollBar::set_visible_fraction()).
     */
    void _update_handle();



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Keeps the handle's size/position in sync with this track's own size. */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;

    /**
     * Jumps the handle to the clicked position, unless the click actually landed
     * on the handle itself, in which case this just grabs it in place (see
     * _dragging_handle) instead, the same way grabbing any other GUI's scrollbar
     * thumb/slider handle doesn't itself move it.
     */
    void _on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position) override;

    /**
     * Drags the handle to wherever the cursor now is, or, if the handle itself was
     * what got grabbed (see _dragging_handle), moves it by the cursor's own delta
     * instead, preserving wherever it was grabbed.
     */
    void _on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position) override;

    /** Left/Down/Right/Up nudge the value by get_step(); Home/End jump to the min/max. */
    void _on_key_down(xl7::input::Key key) override;

    /** Nudges the value by get_step() per wheel notch, while hovered. */
    void _on_mouse_wheel(int delta) override;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /** Returns how far the handle can travel along the main axis (the track's own main-axis size minus the handle's). */
    float _get_track_range() const;

    /** Returns the value the handle would represent if its center were at local_main (along the main axis). */
    float _value_at(float local_main) const;

    /** Sets the value the handle would represent if its center were at local_main (along the main axis). */
    void _set_value_from_local_main(float local_main);



    // #############################################################################
    // Attributes
    // #############################################################################

    Orientation _orientation;

    float _min;
    float _max;
    float _value;

    /** The amount Left/Right/Up/Down nudge the value by. */
    float _step;

    /**
     * Whether the gesture currently in progress (if any) started with a
     * mouse-down directly on the handle, see _on_mouse_down()/_on_mouse_drag():
     * that grabs the handle in place (no immediate jump) and then, while dragging,
     * moves it by the cursor's own delta, preserving wherever it was grabbed,
     * rather than snapping its center to the cursor the way a plain track
     * click/drag does.
     */
    bool _dragging_handle = false;

    cl7::behavioral::Signal<float> _changed;

}; // class TrackControl



} // namespace fl7::gui

#endif // FL7_GUI_TRACKCONTROL_H
