#ifndef FL7_GUI_COMPOUND_H
#define FL7_GUI_COMPOUND_H
#include "./Collection.h"



namespace fl7::gui {



/**
 * A Collection with fixed, widget-specific internal composition (e.g.
 * ComboBox = TextField + Button + Popup; ScrollBar = track + thumb; Window/
 * Frame/Dialog = title bar + content area + buttons). Deliberately does
 * *not* re-expose add_child publicly like Container does -- a concrete
 * Compound subclass builds its own fixed internal parts (via the inherited
 * protected add_child, in its own constructor), and a caller can't add
 * arbitrary children onto it directly. If a Compound wants to accept
 * arbitrary caller content too (e.g. Window's content area), it should
 * expose that narrowly and by name (e.g. get_content_area()), not via a
 * generic public add_child.
 */
class Compound
    : public Collection
{

public:
    Compound() = default;
    ~Compound() override = default;

    Compound(const Compound&) = delete;
    Compound& operator=(const Compound&) = delete;
    Compound(Compound&&) = delete;
    Compound& operator=(Compound&&) = delete;

}; // class Compound



} // namespace fl7::gui

#endif // FL7_GUI_COMPOUND_H
