
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/Button.h>
#include <FaceLabs/gui/faces/Panel.h>

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
