#ifndef FL7_FONTS_POINT_H
#define FL7_FONTS_POINT_H

#include <CoreLabs/root.h>



namespace fl7::fonts {



/**
 * Represents a single point in a glyph's outline.
 */
struct Point
{
    /** The x-coordinate of the point (in font units). */
    float x;
    /** The y-coordinate of the point (in font units). */
    float y;
    /** True if this point lies on the curve; false if it is a control point. */
    bool on_curve;
};



} // namespace fl7::fonts

#endif // FL7_FONTS_POINT_H
