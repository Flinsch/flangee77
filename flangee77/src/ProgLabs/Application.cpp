#include "Application.h"

#include <XiaoLabs/MainWindow.h>
#include <XiaoLabs/graphics.h>

#include <CoreLabs/creational/Singleton.h>
#include <CoreLabs/system/CPUID.h>
#include <CoreLabs/system/MemoryStatus.h>
#include <CoreLabs/profiling.h>
//#include <CoreLabs/logging/FileLogHandler.h>
#include <CoreLabs/logging/HtmlLogHandler.h>
#include <CoreLabs/logging.h>
#include <CoreLabs/memory.h>



namespace pl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Application::Application(ArgumentBag argument_bag)
        : _argument_bag(std::move(argument_bag))
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Runs the application instance.
     * Initializes the framework components, starts the main message loop, and shuts
     * down the framework components.
     */
    bool Application::run()
    {
        bool ok = true;

        while (ok)
        {
            _quit_flag = false;
            _restart_flag = false;

            ok = _init();
            ok = ok && _run_loop();
            ok = _shutdown() && ok;

            if (ok && _restart_flag)
                continue;
            break;
        } // while (ok)

        return ok;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Initializes the application instance including the framework components.
     */
    bool Application::_init()
    {
        // Create/replace log handler.
        //auto log_handler = std::make_shared<cl7::logging::FileLogHandler>();
        auto log_handler = std::make_shared<cl7::logging::HtmlLogHandler>();
        cl7::logging::StandardLogger::instance().clear_log_handlers().add_log_handler(log_handler);

        // "Touch" the "X" pre-config provider
        // and get the "default" settings.
        auto config = xl7::config_provider().get_config();

        // Perform "custom" pre-initialization.
        if (!_pre_init_impl(config))
            return false;

        // Write the settings back to the provider.
        xl7::config_provider().set_config(config);

        // Print out CPU identification/information.
        LOG_TYPE(u8"CPU identification/information:", cl7::logging::LogType::Caption);
        cl7::system::CPUID cpuid;
        if (!cpuid.capture())
            LOG_WARNING(u8"Unable to retrieve CPU identification/information.");
        LOG_TYPE(u8"Vendor name\t" + cl7::u8string{cpuid.vendor_name}, cl7::logging::LogType::Item);
        LOG_TYPE(u8"Processor name\t" + cl7::u8string{cpuid.processor_name}, cl7::logging::LogType::Item);
        if constexpr (std::endian::native == std::endian::little)
            LOG_TYPE(u8"Endianness\tlittle endian", cl7::logging::LogType::Item);
        if constexpr (std::endian::native == std::endian::big)
            LOG_TYPE(u8"Endianness\tbig endian", cl7::logging::LogType::Item);
        if (cpuid.bitness && cpuid.bitness != sizeof(size_t) * 8)
            LOG_TYPE(u8"Bitness\t" + cl7::to_string(cpuid.bitness) + u8"-bit" + u8" (application: " + cl7::to_string(sizeof(size_t) * 8) + u8"-bit" + u8")", cl7::logging::LogType::Item);
        else
            LOG_TYPE(u8"Bitness\t" + (cpuid.bitness ? cl7::to_string(cpuid.bitness) + u8"-bit" : cl7::u8string(u8"unknown")), cl7::logging::LogType::Item);
        LOG_TYPE(u8"Frequency\t" + (cpuid.frequency ? cl7::to_string(cpuid.frequency) + u8" MHz" : cl7::u8string(u8"unknown")), cl7::logging::LogType::Item);
        LOG_TYPE(u8"Concurrency\t" + (cpuid.hardware_concurrency ? cl7::to_string(cpuid.hardware_concurrency) + u8" thread contexts" : cl7::u8string(u8"unknown")), cl7::logging::LogType::Item);

        // Print out the system memory status.
        LOG_TYPE(u8"System memory status:", cl7::logging::LogType::Caption);
        cl7::system::MemoryStatus memory_status;
        if (!memory_status.capture())
            LOG_WARNING(u8"Unable to retrieve system memory status.");
        LOG_TYPE(u8"Total physical memory\t" + cl7::memory::stringify_byte_amount_si(memory_status.total_physical_memory), cl7::logging::LogType::Item);
        LOG_TYPE(u8"Available physical memory\t" + cl7::memory::stringify_byte_amount_si(memory_status.available_physical_memory), cl7::logging::LogType::Item);

        // 
        if constexpr (sizeof(size_t) == 4)
            LOG_TYPE(u8"Apologies for sticking to 32-bit and thus limiting usable memory. But this software doesn't need more either. \U0001f618", cl7::logging::LogType::Comment);

        // Create the main window.
        if (!xl7::main_window().init())
            return false;

        // Initialize the graphics system.
        if (!xl7::graphics::graphics_system().init())
            return false;

        // Show the main window.
        xl7::main_window().show_window();

        // Perform "custom" post-initialization.
        return _post_init_impl();
    }

    /**
     * Shuts down the application instance including the framework components.
     */
    bool Application::_shutdown()
    {
        // "Dump" the runtime performance profile.
        LOG_TYPE(u8"Runtime performance profile:", cl7::logging::LogType::Caption);
        cl7::profiling::StandardRegistry::instance().dump(&cl7::logging::StandardLogger::instance());

        // Perform "custom" shutdown.
        bool ok = _shutdown_impl();

        // Close the main window.
        xl7::main_window().close();

        // Destroy all singleton objects
        // (also shutting down all "X" components).
        cl7::creational::SingletonManager::destroy_all();

        return ok;
    }

    /**
     * Enters/runs the main message loop of the application instance.
     */
    bool Application::_run_loop()
    {
        while (true)
        {
            const auto [quit_flag, exit_code] = xl7::MainWindow::process_window_messages();

            if (quit_flag)
            {
                _quit_flag = true;
                _exit_code = exit_code;
            }

            if (_quit_flag || _restart_flag)
                break;

            

            // Pause the application logic while the
            // rendering device is lost and not reset.
            if (xl7::graphics::rendering_device()->check_device_lost())
            {
                if (!xl7::graphics::rendering_device()->handle_device_lost())
                    continue;
            }

            

            // Start the stopwatch.
            cl7::profiling::Stopwatch stopwatch{true};

            // Perform application loop iteration.
            _loop();

            // Stop the stopwatch.
            stopwatch.stop();

            // Update the profiler.
            cl7::profiling::StandardRegistry::instance().end_frame_and_update_stats(stopwatch.calculate_elapsed_usecs());
        } // while (true)

        return true;
    }

    /**
     * Does a full application loop iteration.
     * Therefore, it clears the render targets; prepares rendering; begins the
     * scene; performs the actual rendering; ends the scene; presents the rendered
     * scene by flipping the swap chain; and performs the application logic.
     * (See _before_render, _render, _after_render, _present, and _move.)
     */
    void Application::_loop()
    {
        cl7::profiling::Profiler profiler(u8"Application::_loop");

        // We (first) render the complete scene (without "flipping" the swap chain),
        // (second) perform CPU calculations, and (third) present the scene (now by
        // "flipping" the swap chain). Having CPU calculations between rendering and
        // presentation, we try to make the most of the time the graphics card still
        // has to finish processing.
        _before_render();
        _render();
        _after_render();
        _move();
        _present();
    }

    /**
     * Begins the scene and prepares rendering.
     */
    void Application::_before_render()
    {
        cl7::profiling::Profiler profiler(u8"Application::_before_render");

        // Update resources etc.?

        //xl7::graphics::rendering_device()->clear_buffers();
        xl7::graphics::primary_context()->begin_scene();

        _before_render_impl();
    }

    /**
     * Performs the actual rendering.
     */
    void Application::_render()
    {
        cl7::profiling::Profiler profiler(u8"Application::_render");

        _render_impl();

        // Draw FPS etc.?
    }

    /**
     * Performs follow-up stuff and ends the scene.
     */
    void Application::_after_render()
    {
        cl7::profiling::Profiler profiler(u8"Application::_after_render");

        _after_render_impl();

        xl7::graphics::primary_context()->end_scene();
    }

    /**
     * Presents the rendered scene by "flipping" the swap chain.
     */
    void Application::_present() // NOLINT(*-convert-member-functions-to-static)
    {
        cl7::profiling::Profiler profiler(u8"Application::_present");

        xl7::graphics::rendering_device()->present();
    }

    /**
     * Performs the application logic.
     */
    void Application::_move()
    {
        cl7::profiling::Profiler profiler(u8"Application::_move");

        // Update input devices etc.?

        _move_impl();
    }



} // namespace pl7
