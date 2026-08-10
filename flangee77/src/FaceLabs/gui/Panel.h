#ifndef FL7_GUI_PANEL_H
#define FL7_GUI_PANEL_H
#include "./Container.h"
#include "./HasBackground.h"



namespace fl7::gui {



/**
 * A plain background/border chrome (see Style) around arbitrary, publicly addable
 * children: the `Container`-based counterpart to `Window`'s `Compound`-based fixed
 * content area. Purely a marker beyond that: no new members, just a distinct type
 * so a renderer can tell "this container also wants a background drawn" from a bare
 * `Container` that doesn't.
 */
class Panel
    : public Container
    , public HasBackground
{

public:
    Panel() = default;

    Panel(const Panel&) = delete;
    Panel& operator=(const Panel&) = delete;
    Panel(Panel&&) = delete;
    Panel& operator=(Panel&&) = delete;

    ~Panel() override = default;



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this panel's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"panel"; }

}; // class Panel



} // namespace fl7::gui

#endif // FL7_GUI_PANEL_H
