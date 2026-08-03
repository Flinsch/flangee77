#ifndef FL7_GUI_PANEL_H
#define FL7_GUI_PANEL_H
#include "./Container.h"



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
{

public:
    Panel() = default;

    Panel(const Panel&) = delete;
    Panel& operator=(const Panel&) = delete;
    Panel(Panel&&) = delete;
    Panel& operator=(Panel&&) = delete;

    ~Panel() override = default;

}; // class Panel



} // namespace fl7::gui

#endif // FL7_GUI_PANEL_H
