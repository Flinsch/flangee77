#include "Rect.h"



namespace al7::packing {



    bool Rect::overlaps(const Rect& other) const noexcept
    {
        return position.x < other.position.x + other.size.width
            && position.y < other.position.y + other.size.height
            && position.x + size.width > other.position.x
            && position.y + size.height > other.position.y
        ;
    }

    bool Rect::contains(const Rect& other) const noexcept
    {
        return position.x <= other.position.x
            && position.y <= other.position.y
            && position.x + size.width >= other.position.x + other.size.width
            && position.y + size.height >= other.position.y + other.size.height
        ;
    }



} // namespace al7::packing
