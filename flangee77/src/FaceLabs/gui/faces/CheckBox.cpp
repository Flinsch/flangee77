#include "CheckBox.h"

#include "../Face.h"
#include "../HasBackground.h"

#include <algorithm>



namespace fl7::gui::faces {



    // #############################################################################
    // CheckBox::Box
    // #############################################################################

    /**
     * The little checkbox glyph itself: a passive HasBackground box, themed
     * "checkbox". Disabled (excluded from hit-testing), so every click/hover across
     * the whole CheckBox (box + caption) resolves to the CheckBox itself, never to
     * the box alone. Mirrors the owning CheckBox's checked/hovered/pressed/focused
     * state for theme resolution via _get_interaction_state_proxy() instead of its
     * own: it isn't itself a HasCheckedState, and (being disabled) is never actually
     * hit-tested, hovered, pressed, or focused on its own.
     */
    class CheckBox::Box
        : public Face
        , public HasBackground
    {
    public:
        Box() { set_enabled(false); }

    protected:
        cl7::u8string_view _get_theme_key() const override { return u8"checkbox"; }
        const Face* _get_interaction_state_proxy() const override { return get_parent(); }
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    CheckBox::CheckBox(bool checked, cl7::u32string text)
        : _box(add_child<Box>())
        , _label(add_child<Label>(std::move(text)))
        , _checked(checked)
    {
        _label.set_enabled(false);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets whether this checkbox is checked, unless already in that state.
     */
    void CheckBox::set_checked(bool checked)
    {
        if (_checked == checked)
            return;

        _checked = checked;
        _changed.emit(_checked);
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Keeps the box/caption layout in sync with this checkbox's own size.
     */
    void CheckBox::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        constexpr float caption_gap = 6.0f;

        const float box_side = new_size.y;
        _box.set_position({0.0f, 0.0f});
        _box.set_size({box_side, box_side});

        _label.set_position({box_side + caption_gap, 0.0f});
        _label.set_size({std::max(0.0f, new_size.x - box_side - caption_gap), new_size.y});
    }



} // namespace fl7::gui::faces
