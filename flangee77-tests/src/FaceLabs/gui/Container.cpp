
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/faces/Button.h>
#include <FaceLabs/gui/faces/Panel.h>
#include <FaceLabs/gui/layout/BoxLayout.h>

#include "./shared.h"



namespace {

    /** A plain Container that flags *destroyed (if set) on destruction, so a test can observe that a removed subtree actually gets torn down, not just unlinked. */
    class TrackedContainer
        : public fl7::gui::Container
    {
    public:
        ~TrackedContainer() override { if (destroyed) *destroyed = true; }

        bool* destroyed = nullptr;
    };

} // namespace



// #############################################################################
// Child order (insertion order, not Z-order)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  Container:  get_children() reflects insertion order; remove_child() removes exactly the targeted child, leaving the others in their original relative order" )
{
    fl7::gui::Container parent;

    auto& child_a = parent.add_child<fl7::gui::Container>();
    auto& child_b = parent.add_child<fl7::gui::Container>();
    auto& child_c = parent.add_child<fl7::gui::Container>();

    TESTLABS_CHECK_EQ( parent.get_children().size(), size_t(3) );
    TESTLABS_CHECK( parent.get_children()[0].get() == static_cast<fl7::gui::Face*>(&child_a) );
    TESTLABS_CHECK( parent.get_children()[1].get() == static_cast<fl7::gui::Face*>(&child_b) );
    TESTLABS_CHECK( parent.get_children()[2].get() == static_cast<fl7::gui::Face*>(&child_c) );

    parent.remove_child(child_b); // the middle one

    TESTLABS_CHECK_EQ( parent.get_children().size(), size_t(2) );
    TESTLABS_CHECK( parent.get_children()[0].get() == static_cast<fl7::gui::Face*>(&child_a) );
    TESTLABS_CHECK( parent.get_children()[1].get() == static_cast<fl7::gui::Face*>(&child_c) );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Container:  remove_child() destroys the removed child along with its own descendants, not just unlinking it" )
{
    fl7::gui::Container parent;

    auto& child = parent.add_child<TrackedContainer>();
    bool child_destroyed = false;
    child.destroyed = &child_destroyed;

    auto& grandchild = child.add_child<TrackedContainer>();
    bool grandchild_destroyed = false;
    grandchild.destroyed = &grandchild_destroyed;

    parent.remove_child(child);

    TESTLABS_CHECK( child_destroyed );
    TESTLABS_CHECK( grandchild_destroyed );
}



// #############################################################################
// Z-order and clipping within a single Collection's own children
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  Container:  of two overlapping children of the same parent, the one added later (topmost in Z-order) wins hit-testing in their overlap" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({200.0f, 200.0f});

    auto& back = panel.add_child<fl7::gui::faces::Panel>();
    back.set_position({0.0f, 0.0f});
    back.set_size({100.0f, 100.0f});

    auto& front = panel.add_child<fl7::gui::faces::Panel>(); // added after back: topmost
    front.set_position({50.0f, 50.0f});
    front.set_size({100.0f, 100.0f}); // overlaps back's bottom-right quadrant

    input.move_mouse_to({75.0f, 75.0f}); // the overlap itself
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&front) );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Container:  clips_children() defaults to true, keeping a child outside this collection's own bounds out of hit-testing entirely; disabling it makes that child reachable again" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({50.0f, 50.0f});

    auto& button = panel.add_child<fl7::gui::faces::Button>(U"Caption");
    button.set_position({100.0f, 0.0f}); // entirely outside the panel's own [0, 50) x [0, 50) bounds
    button.set_size({50.0f, 20.0f});

    TESTLABS_CHECK( panel.clips_children() ); // default

    input.move_mouse_to({125.0f, 10.0f}); // inside the button, but outside the panel
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == nullptr ); // clipped: unreachable, nothing else hit either

    panel.set_clip_children(false);
    shell.update(); // re-evaluates hover every call, even without further input

    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&button) );
}



// #############################################################################
// Preferred/minimum size (bounding box of explicitly placed children; no layout)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  Container:  with no children and no layout, get_effective_preferred_size()/get_effective_minimum_size() are {0, 0}" )
{
    fl7::gui::Container container;
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().y, 0.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Container:  with no layout, get_effective_preferred_size()/get_effective_minimum_size() are the tightest bounding box over every child's own get_position() plus its own effective preferred/minimum size" )
{
    fl7::gui::Container container;

    auto& child_a = container.add_child<fl7::gui::Container>();
    child_a.set_position({10.0f, 10.0f});
    child_a.set_preferred_size({20.0f, 20.0f}); // extends to {30, 30}
    child_a.set_minimum_size({5.0f, 5.0f}); // extends to {15, 15}

    auto& child_b = container.add_child<fl7::gui::Container>();
    child_b.set_position({50.0f, 0.0f});
    child_b.set_preferred_size({40.0f, 10.0f}); // extends to {90, 10}: widest, but not tallest
    child_b.set_minimum_size({40.0f, 10.0f}); // extends to {90, 10}

    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().x, 90.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().y, 30.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().x, 90.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().y, 15.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Container:  the bounding box calculation is recursive: a child Container's own get_effective_preferred_size() is itself the bounding box over ITS children" )
{
    fl7::gui::Container outer;

    auto& inner = outer.add_child<fl7::gui::Container>();
    inner.set_position({5.0f, 5.0f});

    auto& grandchild = inner.add_child<fl7::gui::Container>();
    grandchild.set_position({10.0f, 10.0f});
    grandchild.set_preferred_size({20.0f, 20.0f}); // inner's own preferred size: {30, 30}

    // outer's own: inner's position {5, 5} plus inner's own (recursively calculated) preferred size {30, 30}.
    TESTLABS_CHECK_EQ_FLT( outer.get_effective_preferred_size().x, 35.0f );
    TESTLABS_CHECK_EQ_FLT( outer.get_effective_preferred_size().y, 35.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Container:  the container's own effective padding is added on top of the children's bounding box automatically (by Face::get_effective_preferred_size()/get_effective_minimum_size()), same as for any other face type; _calculate_preferred_size()/_calculate_minimum_size() themselves stay content-only (no layout, so no padding-based inset of the children's own positions)" )
{
    fl7::gui::Style style;
    style.padding = fl7::gui::Padding(4.0f, 2.0f, 4.0f, 2.0f); // left/right 4, top/bottom 2.

    fl7::gui::Container container;
    container.set_style_override(style);

    auto& child = container.add_child<fl7::gui::Container>();
    child.set_position({10.0f, 10.0f});
    child.set_preferred_size({20.0f, 20.0f}); // bounding box: {30, 30}
    child.set_minimum_size({20.0f, 20.0f});

    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().x, 38.0f ); // 30 + left(4) + right(4)
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().y, 34.0f ); // 30 + top(2) + bottom(2)
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().x, 38.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().y, 34.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Container:  once a layout is set, get_effective_preferred_size()/get_effective_minimum_size() defer to its own layout::Layout::calculate_preferred_size()/calculate_minimum_size() instead of the children's bounding box" )
{
    fl7::gui::Container container;

    auto& child = container.add_child<fl7::gui::Container>();
    child.set_position({10.0f, 10.0f});
    child.set_preferred_size({20.0f, 20.0f});
    child.set_minimum_size({5.0f, 5.0f});

    const ml7::Vector2f bounding_box = container.get_effective_preferred_size();
    TESTLABS_CHECK_EQ_FLT( bounding_box.x, 30.0f ); // no layout yet: child's position (10) + its own preferred size (20).
    TESTLABS_CHECK_EQ_FLT( bounding_box.y, 30.0f );

    container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Horizontal);

    // BoxLayout::calculate_preferred_size()/calculate_minimum_size() ignore the
    // child's own get_position() entirely (BoxLayout doesn't care where a child
    // used to sit, only its own preferred/minimum size): with a single child and
    // no gap/padding, both axes just reduce to that child's own preferred/
    // minimum size, a different (and smaller) number than the bounding box above.
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().x, 20.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().y, 20.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().x, 5.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().y, 5.0f );
}



// #############################################################################
// Padding-based child shift (get_scroll_offset(); no layout)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  Container:  with no layout, get_scroll_offset() is this container's own effective padding's left/top, negated; every child's own absolute position ends up shifted inward by that much accordingly; once a layout is attached, get_scroll_offset() reverts to {0, 0}" )
{
    fl7::gui::Style style;
    style.padding = fl7::gui::Padding(4.0f, 2.0f, 4.0f, 2.0f); // left/right 4, top/bottom 2.

    fl7::gui::Container container;
    container.set_style_override(style);
    container.set_position({100.0f, 100.0f});

    TESTLABS_CHECK_EQ_FLT( container.get_scroll_offset().x, -4.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_scroll_offset().y, -2.0f );

    auto& child = container.add_child<fl7::gui::Container>();
    child.set_position({0.0f, 0.0f});

    TESTLABS_CHECK_EQ_FLT( child.get_absolute_position().x, 104.0f ); // 100 + left(4)
    TESTLABS_CHECK_EQ_FLT( child.get_absolute_position().y, 102.0f ); // 100 + top(2)

    container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Horizontal);

    TESTLABS_CHECK_EQ_FLT( container.get_scroll_offset().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_scroll_offset().y, 0.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Container:  the padding-based child shift (no layout) is actually honored by hit-testing, not just get_absolute_position(): a child at local {0, 0} is unreachable just inside this container's own raw edge, but reachable just inside its padded edge" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    fl7::gui::Style style;
    style.padding = fl7::gui::Padding(10.0f); // uniform.

    auto& container = shell.add_face<fl7::gui::Container>();
    container.set_style_override(style);
    container.set_position({0.0f, 0.0f});
    container.set_size({100.0f, 100.0f});

    auto& child = container.add_child<fl7::gui::faces::Button>(U"OK");
    child.set_position({0.0f, 0.0f});
    child.set_size({20.0f, 20.0f});

    // Without the padding shift, the child would start right at the container's
    // own {0, 0}; shifted inward by 10 on every side, it actually starts at
    // {10, 10}, so {5, 5} still falls on the container itself, not the child.
    input.move_mouse_to({5.0f, 5.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&container) );

    input.move_mouse_to({15.0f, 15.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&child) );
}
