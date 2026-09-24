#include "ScrollBar.h"

#include <algorithm>



namespace fl7::gui::faces {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ScrollBar::ScrollBar(Orientation orientation, float visible_fraction, float value, float min, float max, bool show_buttons)
        : _slider(add_child<Slider>(orientation, value, min, max))
        , _decrement_button(add_child<Button>(U"-"))
        , _increment_button(add_child<Button>(U"+"))
        , _orientation(orientation)
        , _show_buttons(show_buttons)
    {
        _slider.set_handle_fraction(std::clamp(visible_fraction, 0.0f, 1.0f));

        _decrement_button.get_clicked().connect([this]() {
            _slider.set_value(_slider.get_value() - _slider.get_step());
        });
        _increment_button.get_clicked().connect([this]() {
            _slider.set_value(_slider.get_value() + _slider.get_step());
        });
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets how much of the larger content this scroll bar represents is currently
     * visible, clamped to (0, 1].
     */
    void ScrollBar::set_visible_fraction(float visible_fraction)
    {
        _slider.set_handle_fraction(std::clamp(visible_fraction, 0.0f, 1.0f));
    }

    /**
     * Sets whether the two step buttons are shown at all; hidden, the slider fills
     * the whole length instead.
     */
    void ScrollBar::set_show_buttons(bool show_buttons)
    {
        if (show_buttons == _show_buttons)
            return;

        _show_buttons = show_buttons;
        _layout();
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Keeps the slider/buttons' sizes/positions in sync with this scroll bar's own size.
     */
    void ScrollBar::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        _layout();
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Lays out the step buttons (if shown) at each end and the slider filling
     * whatever's left between them.
     */
    void ScrollBar::_layout()
    {
        const bool horizontal = _orientation == Orientation::Horizontal;
        const float main = horizontal ? get_size().x : get_size().y;
        const float cross = horizontal ? get_size().y : get_size().x;
        const float button_main = _show_buttons ? std::min(cross, main * 0.5f) : 0.0f;

        const auto make_vector = [horizontal](float m, float c) { return horizontal ? ml7::Vector2f{m, c} : ml7::Vector2f{c, m}; };

        _decrement_button.set_visible(_show_buttons);
        _decrement_button.set_enabled(_show_buttons);
        _decrement_button.set_position(make_vector(0.0f, 0.0f));
        _decrement_button.set_size(make_vector(button_main, cross));

        const float slider_main = std::max(0.0f, main - 2.0f * button_main);
        _slider.set_position(make_vector(button_main, 0.0f));
        _slider.set_size(make_vector(slider_main, cross));

        _increment_button.set_visible(_show_buttons);
        _increment_button.set_enabled(_show_buttons);
        _increment_button.set_position(make_vector(button_main + slider_main, 0.0f));
        _increment_button.set_size(make_vector(button_main, cross));
    }



} // namespace fl7::gui::faces
