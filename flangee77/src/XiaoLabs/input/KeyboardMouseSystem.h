#ifndef XL7_INPUT_KEYBOARDMOUSESYSTEM_H
#define XL7_INPUT_KEYBOARDMOUSESYSTEM_H
#include "../DriverBasedComponent.h"

#include "./Keyboard.h"
#include "./Mouse.h"

#include <array>
#include <chrono>
#include <cstdint>
#include <memory>
#include <vector>



namespace xl7::input {



class KeyboardMouseSystem
    : public DriverBasedComponent<KeyboardMouseSystem>
{
    friend class cl7::creational::Singleton<KeyboardMouseSystem>;

public:
    /**
     * Opaque, backend-assigned identity for a physical keyboard/mouse device
     * (e.g., a Raw Input HANDLE), used to find the right Keyboard/Mouse
     * instance again on a later message/hotplug event.
     */
    using DeviceId = std::uint64_t;



    KeyboardMouseSystem(const KeyboardMouseSystem&) = delete;
    KeyboardMouseSystem& operator=(const KeyboardMouseSystem&) = delete;
    KeyboardMouseSystem(KeyboardMouseSystem&&) = delete;
    KeyboardMouseSystem& operator=(KeyboardMouseSystem&&) = delete;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the number of currently known individual keyboards.
     */
    unsigned get_keyboard_count() const { return static_cast<unsigned>(_keyboards.size()); }

    /**
     * Returns the specified individual keyboard (0-based).
     */
    Keyboard& get_keyboard(unsigned index) const { assert(index < _keyboards.size()); return *_keyboards[index].device; }

    /**
     * Returns the aggregate of all connected keyboards: a key is "down" if at
     * least one keyboard reports it down. Also the only Keyboard instance
     * whose get_text_input() is ever populated in practice, since composed
     * text input (WM_CHAR on Windows) isn't attributable to a source device.
     */
    Keyboard& aggregated_keyboard() const { return *_aggregated_keyboard; }

    /**
     * Returns the number of currently known individual mice.
     */
    unsigned get_mouse_count() const { return static_cast<unsigned>(_mice.size()); }

    /**
     * Returns the specified individual mouse (0-based).
     */
    Mouse& get_mouse(unsigned index) const { assert(index < _mice.size()); return *_mice[index].device; }

    /**
     * Returns the aggregate of all connected mice: a button is "down" if at
     * least one mouse reports it down; movement and wheel deltas are summed.
     */
    Mouse& aggregated_mouse() const { return *_aggregated_mouse; }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Refreshes the aggregate keyboard/mouse from the current state of all
     * individual devices. Must be called once per frame.
     */
    void update();



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    KeyboardMouseSystem();
    ~KeyboardMouseSystem() override = default;



    // #############################################################################
    // Factory Function
    // #############################################################################

    static KeyboardMouseSystem* factory_func();



    // #############################################################################
    // Backend Interface
    // #############################################################################
    // Real Raw Input device enumeration/hotplug and message handling are not
    // implemented yet (see the "impl/rawinput" backend, currently a stub);
    // these exist so that work can slot in later without another redesign.

    /**
     * Registers a newly detected keyboard and returns the (initially empty)
     * instance for the backend to keep feeding.
     */
    Keyboard& _add_keyboard(DeviceId id);

    /**
     * Unregisters a keyboard that has been disconnected.
     */
    void _remove_keyboard(DeviceId id);

    /**
     * Registers a newly detected mouse and returns the (initially empty)
     * instance for the backend to keep feeding.
     */
    Mouse& _add_mouse(DeviceId id);

    /**
     * Unregisters a mouse that has been disconnected.
     */
    void _remove_mouse(DeviceId id);

    /**
     * Feeds newly observed levels/deltas into an individual keyboard/mouse
     * instance. A protected bridge rather than direct Keyboard::Attorney /
     * Mouse::Attorney access, because backends derive from this class, and
     * friendship (granted to KeyboardMouseSystem itself) isn't inherited by
     * subclasses.
     */
    static void _apply_keyboard(Keyboard& keyboard, const std::array<bool, static_cast<size_t>(Key::COUNT)>& levels, const cl7::u32string& text_input_delta, std::chrono::steady_clock::time_point now);

    /**
     * Feeds newly observed levels/deltas into an individual mouse instance.
     * See _apply_keyboard() for why this bridge exists.
     */
    static void _apply_mouse(Mouse& mouse, const std::array<bool, static_cast<size_t>(MouseButton::COUNT)>& levels, int delta_x, int delta_y, int wheel_delta, std::chrono::steady_clock::time_point now);

    /**
     * Queues composed text input to be reported by the aggregate keyboard on the
     * next update() (see aggregated_keyboard()'s docs for why this isn't attributed
     * to an individual keyboard). UTF-32, per the framework's "user interaction"
     * convention, see Keyboard::get_text_input().
     */
    void _queue_text_input(const cl7::u32string& text);



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Performs backend-specific initialization (e.g., registering for raw input
     * devices).
     */
    virtual bool _init_impl() = 0;

    /**
     * Performs backend-specific de-initialization.
     */
    virtual bool _shutdown_impl() = 0;



    // #############################################################################
    // Component Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool _init() final;

    /**
     * De-initializes the component.
     */
    bool _shutdown() final;



    // #############################################################################
    // Attributes
    // #############################################################################

    struct KeyboardEntry
    {
        DeviceId id = 0;
        std::unique_ptr<Keyboard> device;
    };

    struct MouseEntry
    {
        DeviceId id = 0;
        std::unique_ptr<Mouse> device;
    };

    std::vector<KeyboardEntry> _keyboards;
    std::vector<MouseEntry> _mice;

    std::unique_ptr<Keyboard> _aggregated_keyboard;
    std::unique_ptr<Mouse> _aggregated_mouse;

    cl7::u32string _pending_text_input;

}; // class KeyboardMouseSystem



} // namespace xl7::input

#endif // XL7_INPUT_KEYBOARDMOUSESYSTEM_H
