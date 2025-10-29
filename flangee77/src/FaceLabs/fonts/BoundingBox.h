#ifndef FL7_FONTS_BOUNDINGBOX_H
#define FL7_FONTS_BOUNDINGBOX_H

#include <MathLabs/Vector2.h>



namespace fl7::fonts {



/**
 * Represents the bounding box of glyphs or other related elements and is defined as
 * a rectangle with a lower left corner and an upper right corner, as well as a size
 * derived from these corners.
 */
struct BoundingBox
{
    /** The lower left corner of the bounding rectangle. */
    ml7::Vector2f lower_left;
    /** The upper right corner of the bounding rectangle. */
    ml7::Vector2f upper_right;
    /** The size (width and height) of the bounding rectangle. */
    ml7::Vector2f size;



    BoundingBox() noexcept = default;
    BoundingBox(ml7::Vector2f lower_left, ml7::Vector2f upper_right) noexcept
        : lower_left(lower_left)
        , upper_right(upper_right)
        , size(upper_right - lower_left)
    {
    }
};



} // namespace fl7::fonts

#endif // FL7_FONTS_BOUNDINGBOX_H
