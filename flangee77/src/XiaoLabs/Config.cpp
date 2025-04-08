#include "Config.h"



namespace xl7 {



    Config::Config()
    {
        generic.title = u8"flangee77";

        //generic.console_key = VK_OEM_5;
        generic.console_key = VK_F1;
        generic.quit_key = VK_F10;
        generic.use_hardware_cursor = false;

        video.presentation_mode = Video::PresentationMode::BorderlessFullscreen;
        video.back_buffer_width = 0;
        video.back_buffer_height = 0;
        video.back_buffer_count = 2;
        video.refresh_rate = 0;
        video.vsync_enabled = false;
        video.allow_tearing = true;
    }



} // namespace xl7
