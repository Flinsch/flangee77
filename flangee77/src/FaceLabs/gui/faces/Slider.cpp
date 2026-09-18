#include "Slider.h"

#include "../Face.h"
#include "../HasBackground.h"

#include <algorithm>



namespace fl7::gui::faces {



    // #############################################################################
    // Slider::Handle
    // #############################################################################

    /**
     * The draggable handle itself: a passive HasBackground box, themed
     * "slider_handle". Disabled (excluded from hit-testing), so every click/hover
     * across the whole Slider (track + handle) resolves to the Slider itself,
     * never to the handle alone. Mirrors the owning Slider's hovered/pressed/
     * focused state for theme resolution via _get_interaction_state_proxy()
     * instead of its own: it isn't itself hit-tested, hovered, pressed, or
     * focused on its own.
     */
    class Slider::Handle
        : public Face
        , public HasBackground
    {
    public:
        Handle() { set_enabled(false); }

    protected:
        cl7::u8string_view _get_theme_key() const override { return u8"slider_handle"; }
        const Face* _get_interaction_state_proxy() const override { return get_parent(); }
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Slider::Slider(float value, float min, float max)
        : _handle(add_child<Handle>())
        , _min(min)
        , _max(max)
        , _value(std::clamp(value, min, max))
        , _step((max - min) * 0.01f)
    {
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets this slider's value, clamped to [get_min(), get_max()], unless
     * already at that (clamped) value. Emits the "changed" signal (with the
     * new value) when actually changed.
     */
    void Slider::set_value(float value)
    {
        value = std::clamp(value, _min, _max);
        if (value == _value)
            return;

        _value = value;
        _update_handle();
        _changed.emit(_value);
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Keeps the handle's size/position in sync with this slider's own size.
     */
    void Slider::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        _update_handle();
    }

    /**
     * Jumps the handle to the clicked position.
     */
    void Slider::_on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position)
    {
        _set_value_from_local_x(local_position.x);
    }

    /**
     * Drags the handle to wherever the cursor now is.
     */
    void Slider::_on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position)
    {
        _set_value_from_local_x(local_position.x);
    }

    /**
     * Left/Down/Right/Up nudge the value by get_step(); Home/End jump to the min/max.
     */
    void Slider::_on_key_down(xl7::input::Key key)
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



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns the value the handle would represent if its center were at local_x.
     */
    float Slider::_value_at(float local_x) const
    {
        const float handle_size = get_size().y;
        const float track_range = std::max(0.0f, get_size().x - handle_size);
        if (track_range <= 0.0f)
            return _min;

        const float handle_left = std::clamp(local_x - handle_size * 0.5f, 0.0f, track_range);
        return _min + (handle_left / track_range) * (_max - _min);
    }

    /**
     * Sets the value the handle would represent if its center were at local_x.
     */
    void Slider::_set_value_from_local_x(float local_x)
    {
        set_value(_value_at(local_x));
    }

    /**
     * Resizes/repositions the handle for the current size/value.
     */
    void Slider::_update_handle()
    {
        const float handle_size = get_size().y;
        const float track_range = std::max(0.0f, get_size().x - handle_size);
        const float t = (_max > _min) ? (_value - _min) / (_max - _min) : 0.0f;

        _handle.set_size({handle_size, handle_size});
        _handle.set_position({t * track_range, 0.0f});
    }



} // namespace fl7::gui::faces
