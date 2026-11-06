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
                    size.x = std::max(0.0f, container_size.x - insets.left - insets.right);
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
                    size.y = std::max(0.0f, container_size.y - insets.top - insets.bottom);
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



} // namespace fl7::gui::layout
