#ifndef FL7_GUI_FACES_SLIDER_H
#define FL7_GUI_FACES_SLIDER_H
#include "../TrackControl.h"

#include <optional>



namespace fl7::gui::faces {



/**
 * A TrackControl (horizontal or vertical) with a fixed, square handle by default
 * (see TrackControl for the shared value/step model and click/drag/keyboard/wheel
 * interaction). Set a handle fraction (see set_handle_fraction()) to instead give
 * it a handle whose length itself is meaningful, e.g., ScrollBar builds its middle,
 * draggable part from exactly that.
 */
class Slider
    : public TrackControl
{

public:
    explicit Slider(Orientation orientation = Orientation::Horizontal, float value = 0.0f, float min = 0.0f, float max = 1.0f);

    Slider(const Slider&) = delete;
    Slider& operator=(const Slider&) = delete;
    Slider(Slider&&) = delete;
    Slider& operator=(Slider&&) = delete;

    ~Slider() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the handle's length as a fraction of this slider's own main-axis
     * size, or nullopt for a fixed square handle (the default).
     */
    std::optional<float> get_handle_fraction() const { return _handle_fraction; }

    /**
     * Sets the handle's length as a fraction of this slider's own main-axis size,
     * or nullopt to go back to a fixed square handle.
     */
    void set_handle_fraction(std::optional<float> handle_fraction);



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this slider's (track's) theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"slider"; }



    // #############################################################################
    // TrackControl Implementations
    // #############################################################################

    /** Returns this slider's handle. */
    Face& _get_handle() override;

    /** get_handle_fraction() of this slider's own main-axis size, if set; else the inherited fixed square. */
    float _get_handle_main_size() const override;



private:
    class Handle; // Face + HasBackground only, defined in Slider.cpp; passive, see Handle's own doc comment.

    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Handle& _handle;

    std::optional<float> _handle_fraction;

}; // class Slider



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_SLIDER_H
