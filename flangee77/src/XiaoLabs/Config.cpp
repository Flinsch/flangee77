#include "Config.h"



namespace xl7 {



    /** Default constructor. */
    Config::Config(void)
    {
        generic.window.title = TEXT("flangee77");
        generic.window.icon_handle = NULL;
        generic.window.small_icon_handle = NULL;

        //generic.console_key = VK_OEM_5;
        generic.console_key = VK_F1;
        generic.quit_key = VK_F10;
        generic.use_hardware_cursor = false;

        video.display_mode.fullscreen = false;
        video.display_mode.width = 0;
        video.display_mode.height = 0;
    }



} // namespace xl7
