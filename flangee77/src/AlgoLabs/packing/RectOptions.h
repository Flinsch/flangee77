#ifndef AL7_PACKING_RECTOPTIONS_H
#define AL7_PACKING_RECTOPTIONS_H

#include <CoreLabs/root.h>



namespace al7::packing {



struct RectOptions
{
    /**
     * The uniform padding to be applied as additional space on all four sides when
     * inserting new rectangular items.
     */
    unsigned padding = 0;

    /**
     * Specifies whether the width and height of a rectangular item to be inserted
     * may be swapped in order to insert it (possibly more sensibly or at all).
     */
    bool allow_flip = false;
};



} // namespace al7::packing

#endif // AL7_PACKING_RECTOPTIONS_H
