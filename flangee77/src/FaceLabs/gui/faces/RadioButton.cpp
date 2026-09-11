#include "RadioButton.h"

#include "../Face.h"
#include "../HasBackground.h"
#include "../RadioGroup.h"

#include <algorithm>



namespace fl7::gui::faces {



    // #############################################################################
    // RadioButton::Box
    // #############################################################################

    /**
     * The little radio glyph itself: a passive HasBackground box, themed
     * "radio_button". Disabled (excluded from hit-testing), so every click/hover
     * across the whole RadioButton (box + caption) resolves to the RadioButton
     * itself, never to the box alone. Mirrors the owning RadioButton's checked/
     * hovered/pressed/focused state for theme resolution via
     * _get_interaction_state_proxy() instead of its own: it isn't itself a
     * HasCheckedState, and (being disabled) is never actually hit-tested,
     * hovered, pressed, or focused on its own.
     */
    class RadioButton::Box
        : public Face
        , public HasBackground
    {
    public:
        Box() { set_enabled(false); }

    protected:
        cl7::u8string_view _get_theme_key() const override { return u8"radio_button"; }
        const Face* _get_interaction_state_proxy() const override { return get_parent(); }
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    RadioButton::RadioButton(RadioGroup* group, bool checked, cl7::u32string text)
        : _box(add_child<Box>())
        , _label(add_child<Label>(std::move(text)))
        , _group(group)
    {
        _label.set_enabled(false);

        assert(_group);

        if (checked)
            set_checked(true);
    }

    RadioButton::~RadioButton()
    {
        _group->_forget(this);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets whether this radio button is checked, unless already in that state.
     * Checking it unchecks whichever other member of its group was previously
     * checked. Explicitly unchecking it (unlike a direct click, see _on_click())
     * is still allowed, e.g. to start a group out with nothing selected. Emits
     * the "changed" signal (with the new value) when actually changed.
     */
    void RadioButton::set_checked(bool checked)
    {
        if (_checked == checked)
            return;

        _checked = checked;

        if (_checked)
            _group->_notify_checked(this);
        else
            _group->_forget(this);

        _changed.emit(_checked);
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Keeps the box/caption layout in sync with this radio button's own size.
     */
    void RadioButton::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        constexpr float caption_gap = 6.0f;

        const float box_side = new_size.y;
        _box.set_position({0.0f, 0.0f});
        _box.set_size({box_side, box_side});

        _label.set_position({box_side + caption_gap, 0.0f});
        _label.set_size({std::max(0.0f, new_size.x - box_side - caption_gap), new_size.y});
    }



    // #############################################################################
    // Backend Interface (RadioGroup)
    // #############################################################################

    /**
     * Unchecks this radio button directly, without notifying _group (called by
     * RadioGroup itself, once it already knows about the newly checked member, to
     * avoid recursing back into it).
     */
    void RadioButton::_force_unchecked()
    {
        if (!_checked)
            return;

        _checked = false;
        _changed.emit(false);
    }



} // namespace fl7::gui::faces
