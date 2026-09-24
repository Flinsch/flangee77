#include "TrackControl.h"

#include <algorithm>



namespace fl7::gui {



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets this track's value, clamped to [get_min(), get_max()], unless already at
     * that (clamped) value. Emits the "changed" signal (with the new value) when
     * actually changed.
     */
    void TrackControl::set_value(float value)
    {
        value = std::clamp(value, _min, _max);
        if (value == _value)
            return;

        _value = value;
        _update_handle();
        _changed.emit(_value);
    }



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
    TrackControl::TrackControl(Orientation orientation, float value, float min, float max)
        : _orientation(orientation)
        , _min(min)
        , _max(max)
        , _value(std::clamp(value, min, max))
        , _step((max - min) * 0.01f)
    {
    }



    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Returns the handle's size along the main axis (the track's own length).
     * Default: the track's own cross-axis size, i.e., a fixed square handle
     * (Slider's look). Override for a handle whose length is itself meaningful,
     * e.g., ScrollBar: proportional to how much of some larger content is currently
     * visible.
     */
    float TrackControl::_get_handle_main_size() const
    {
        return _cross(get_size());
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Resizes/repositions the handle for the current size/value. Also needed
     * whenever a subclass's own _get_handle_main_size() input changes (see
     * ScrollBar::set_visible_fraction()).
     */
    void TrackControl::_update_handle()
    {
        Face& handle = _get_handle();

        const float main_size = std::clamp(_get_handle_main_size(), 0.0f, _main(get_size()));
        const float cross_size = _cross(get_size());
        const float t = (_max > _min) ? (_value - _min) / (_max - _min) : 0.0f;

        handle.set_size(_make_vector(main_size, cross_size));
        handle.set_position(_make_vector(t * _get_track_range(), 0.0f));
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Keeps the handle's size/position in sync with this track's own size.
     */
    void TrackControl::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        _update_handle();
    }

    /**
     * Jumps the handle to the clicked position, unless the click actually landed
     * on the handle itself, in which case this just grabs it in place (see
     * _dragging_handle) instead, the same way grabbing any other GUI's scrollbar
     * thumb/slider handle doesn't itself move it.
     */
    void TrackControl::_on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position)
    {
        const Face& handle = _get_handle();
        const float local_main = _main(local_position);
        const float handle_main_begin = _main(handle.get_position());
        const float handle_main_end = handle_main_begin + _main(handle.get_size());

        _dragging_handle = local_main >= handle_main_begin && local_main < handle_main_end;

        if (!_dragging_handle)
            _set_value_from_local_main(local_main);
    }

    /**
     * Drags the handle to wherever the cursor now is, or, if the handle itself was
     * what got grabbed (see _dragging_handle), moves it by the cursor's own delta
     * instead, preserving wherever it was grabbed.
     */
    void TrackControl::_on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position)
    {
        if (!_dragging_handle)
        {
            _set_value_from_local_main(_main(local_position));
            return;
        }

        const float track_range = _get_track_range();
        if (track_range > 0.0f)
            set_value(_value + (_main(delta) / track_range) * (_max - _min));
    }

    /**
     * Left/Down/Right/Up nudge the value by get_step(); Home/End jump to the min/max.
     */
    void TrackControl::_on_key_down(xl7::input::Key key)
    {
        using xl7::input::Key;

        switch (key)
        {
        case Key::Left:
        case Key::Down:
            set_value(_value - _step);
            break;

        case Key::Right:
        case Key::Up:
            set_value(_value + _step);
            break;

        case Key::Home:
            set_value(_min);
            break;

        case Key::End:
            set_value(_max);
            break;

        default:
            break;
        }
    }

    /**
     * Nudges the value by get_step() per wheel notch, while hovered.
     */
    void TrackControl::_on_mouse_wheel(int delta)
    {
        // Wheel rotated away from the user (a positive delta/notch, scrolling
        // "up") moves the content up, i.e., decreases the value.
        set_value(_value - static_cast<float>(delta) * _step);
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns how far the handle can travel along the main axis (the track's own main-axis size minus the handle's).
     */
    float TrackControl::_get_track_range() const
    {
        const float main_size = std::clamp(_get_handle_main_size(), 0.0f, _main(get_size()));
        return std::max(0.0f, _main(get_size()) - main_size);
    }

    /**
     * Returns the value the handle would represent if its center were at local_main (along the main axis).
     */
    float TrackControl::_value_at(float local_main) const
    {
        const float main_size = std::clamp(_get_handle_main_size(), 0.0f, _main(get_size()));
        const float track_range = _get_track_range();
        if (track_range <= 0.0f)
            return _min;

        const float handle_main = std::clamp(local_main - main_size * 0.5f, 0.0f, track_range);
        return _min + (handle_main / track_range) * (_max - _min);
    }

    /**
     * Sets the value the handle would represent if its center were at local_main (along the main axis).
     */
    void TrackControl::_set_value_from_local_main(float local_main)
    {
        set_value(_value_at(local_main));
    }



} // namespace fl7::gui
