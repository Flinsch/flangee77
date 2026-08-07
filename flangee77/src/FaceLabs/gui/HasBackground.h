#ifndef FL7_GUI_HASBACKGROUND_H
#define FL7_GUI_HASBACKGROUND_H

#include <CoreLabs/root.h>



namespace fl7::gui {



/**
 * Marker interface: opts a face into having its background drawn by the renderer.
 * Pure marker, no data of its own, the actual style data already lives on the
 * face's style, reachable via the face this is mixed into.
 */
class HasBackground
{

public:
    virtual ~HasBackground() = default;

}; // class HasBackground



} // namespace fl7::gui

#endif // FL7_GUI_HASBACKGROUND_H
