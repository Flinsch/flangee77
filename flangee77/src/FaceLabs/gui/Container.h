#ifndef FL7_GUI_CONTAINER_H
#define FL7_GUI_CONTAINER_H
#include "./Collection.h"



namespace fl7::gui {



/**
 * A Collection with arbitrary, publicly addable children (e.g. Panel,
 * GroupBox). Intended to eventually position its children via a pluggable
 * layout (see the planned gui::layout namespace: DefaultLayout/BoxLayout/
 * GridLayout) -- not implemented yet, children just keep whatever position/
 * size they're given for now.
 */
class Container
    : public Collection
{

public:
    Container() = default;

    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;
    Container(Container&&) = delete;
    Container& operator=(Container&&) = delete;

    ~Container() override = default;



    using Collection::add_child;

}; // class Container



} // namespace fl7::gui

#endif // FL7_GUI_CONTAINER_H
