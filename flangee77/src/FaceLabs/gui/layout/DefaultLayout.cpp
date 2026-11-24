#include "DefaultLayout.h"

#include "../Container.h"
#include "../Face.h"

#include <algorithm>
#include <unordered_set>



namespace fl7::gui::layout {



    // #############################################################################
    // Layout Implementation
    // #############################################################################

    /**
     * (Re-)arranges container's current children within it, per the anchoring rules
     * described on this class.
     */
    void DefaultLayout::perform(Container& container)
    {
        const auto& children = container.get_children();

        // Prune insets for any child no longer actually present.
        std::unordered_set<const Face*> current;
        for (const auto& child : children)
            current.insert(child.get());
        for (auto it = _insets.begin(); it != _insets.end(); )
            it = current.contains(it->first) ? std::next(it) : _insets.erase(it);

        const ml7::Vector2f container_size = container.get_size();

        for (const auto& child_ptr : children)
        {
            Face& child = *child_ptr;

            auto [it, inserted] = _insets.try_emplace(&child);
            Insets& insets = it->second;
            if (inserted)
            {
                const ml7::Vector2f position = child.get_position();
                const ml7::Vector2f size = child.get_size();
                insets.left = position.x;
                insets.top = position.y;
                insets.right = container_size.x - (position.x + size.x);
                insets.bottom = container_size.y - (position.y + size.y);
            }

            const Anchor anchor = child.get_anchor();
            const bool anchor_left = (anchor & Anchor::Left) != Anchor::None;
            const bool anchor_right = (anchor & Anchor::Right) != Anchor::None;
            const bool anchor_top = (anchor & Anchor::Top) != Anchor::None;
            const bool anchor_bottom = (anchor & Anchor::Bottom) != Anchor::None;

            ml7::Vector2f position = child.get_position();
            ml7::Vector2f size = child.get_size();

            if (anchor_right)
            {
                if (anchor_left)
                {
                    position.x = insets.left;
                    // Never below this child's own effective minimum width:
                    // see calculate_minimum_size()'s own doc comment.
                    size.x = std::max(child.get_effective_minimum_size().x, container_size.x - insets.left - insets.right);
                }
                else
                {
                    position.x = container_size.x - insets.right - size.x;
                }
            }
            if (anchor_bottom)
            {
                if (anchor_top)
                {
                    position.y = insets.top;
                    // Never below this child's own effective minimum height:
                    // see calculate_minimum_size()'s own doc comment.
                    size.y = std::max(child.get_effective_minimum_size().y, container_size.y - insets.top - insets.bottom);
                }
                else
                {
                    position.y = container_size.y - insets.bottom - size.y;
                }
            }

            if (position != child.get_position())
                child.set_position(position);
            if (size != child.get_size())
                child.set_size(size);
        }
    }

    /**
     * Returns, per axis, the largest of every Left+Right (or Top+Bottom)
     * filled child's own required container extent for it to sit at its own
     * effective preferred size; see `_calculate_size()`.
     */
    ml7::Vector2f DefaultLayout::calculate_preferred_size(const Container& container) const
    {
        return _calculate_size(container, &Face::get_effective_preferred_size);
    }

    /**
     * Returns, per axis, the largest of every Left+Right (or Top+Bottom)
     * filled child's own required container extent, so the container never actually
     * shrinks any such child below its own effective minimum size; see
     * `_calculate_size()`.
     */
    ml7::Vector2f DefaultLayout::calculate_minimum_size(const Container& container) const
    {
        return _calculate_size(container, &Face::get_effective_minimum_size);
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns, per axis, the largest of every Left+Right (or Top+Bottom) filled
     * child's own required container extent for it to sit at `(child.*get_size)()`:
     * this layout keeps that child's own captured insets fixed (see this class's
     * own doc comment), so the container needs to stay at least
     * `own_current_size - this child's own current size + (child.*get_size)()`
     * wide/tall for that. A child anchored on only one side, or neither, never
     * resizes on that axis to begin with, so it never contributes here either.
     * Shared by `calculate_preferred_size()` and `calculate_minimum_size()`, which,
     * structurally, are otherwise identical.
     */
    ml7::Vector2f DefaultLayout::_calculate_size(const Container& container, ml7::Vector2f (Face::*get_size)() const) const
    {
        const ml7::Vector2f container_size = container.get_size();

        float width = 0.0f;
        float height = 0.0f;

        for (const auto& child_ptr : container.get_children())
        {
            Face& child = *child_ptr;
            const Anchor anchor = child.get_anchor();
            const bool anchor_left = (anchor & Anchor::Left) != Anchor::None;
            const bool anchor_right = (anchor & Anchor::Right) != Anchor::None;
            const bool anchor_top = (anchor & Anchor::Top) != Anchor::None;
            const bool anchor_bottom = (anchor & Anchor::Bottom) != Anchor::None;

            const ml7::Vector2f size = child.get_size();
            const ml7::Vector2f target = (child.*get_size)();

            if (anchor_left && anchor_right)
                width = std::max(width, container_size.x - size.x + target.x);
            if (anchor_top && anchor_bottom)
                height = std::max(height, container_size.y - size.y + target.y);
        }

        return {width, height};
    }



} // namespace fl7::gui::layout
