#include "Slider.h"

#include "../Face.h"
#include "../HasBackground.h"



namespace fl7::gui::faces {



    // #############################################################################
    // Slider::Handle
    // #############################################################################

    /**
     * The draggable handle itself: a passive HasBackground box, themed
     * "slider_handle". Disabled (excluded from hit-testing), so every click/hover
     * across the whole Slider (track + handle) resolves to the Slider itself, never
     * to the handle alone, except hovered, which additionally requires the pointer
     * to really be over the handle's own (current) bounds (see
     * _requires_own_bounds_for_hover()), not just anywhere on the track, so it only
     * highlights when it's actually the thing under the cursor. Mirrors the owning
     * Slider's pressed/focused state for theme resolution via
     * _get_interaction_state_proxy() instead of its own: it isn't itself
     * hit-tested, pressed, or focused on its own.
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
        bool _requires_own_bounds_for_hover() const override { return true; }
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Slider::Slider(Orientation orientation, float value, float min, float max)
        : TrackControl(orientation, value, min, max)
        , _handle(add_child<Handle>())
    {
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets the handle's length as a fraction of this slider's own main-axis size,
     * or nullopt to go back to a fixed square handle.
     */
    void Slider::set_handle_fraction(std::optional<float> handle_fraction)
    {
        _handle_fraction = handle_fraction;
        _update_handle();
    }



    // #############################################################################
    // TrackControl Implementations
    // #############################################################################

    /**
     * Returns this slider's handle.
     */
    Face& Slider::_get_handle()
    {
        return _handle;
    }

    /**
     * get_handle_fraction() of this slider's own main-axis size, if set; else the inherited fixed square.
     */
    float Slider::_get_handle_main_size() const
    {
        if (!_handle_fraction)
            return TrackControl::_get_handle_main_size();

        return _main(get_size()) * *_handle_fraction;
    }



} // namespace fl7::gui::faces
