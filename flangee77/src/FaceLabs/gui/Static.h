#ifndef FL7_GUI_STATIC_H
#define FL7_GUI_STATIC_H
#include "./Face.h"



namespace fl7::gui {



/**
 * Taxonomic base for display-only, non-interactive leaf faces (Label,
 * PictureBox, ProgressBar, Separator). Purely a marker for now -- no new
 * members. Still inherits Face's input-event hooks as no-op defaults: a
 * Static face is free to override them (e.g. a hover tooltip), it just
 * usually won't.
 */
class Static
    : public Face
{

public:
    Static() = default;

    Static(const Static&) = delete;
    Static& operator=(const Static&) = delete;
    Static(Static&&) = delete;
    Static& operator=(Static&&) = delete;

    ~Static() override = default;

}; // class Static



} // namespace fl7::gui

#endif // FL7_GUI_STATIC_H
