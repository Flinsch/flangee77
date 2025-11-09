#include "Config.h"



namespace xl7 {



    Config::Config()
    {
        general.title = u8"flangee77";

        //general.console_key = 0xDC;//VK_OEM_5;
        general.console_key = 0x70;//VK_F1;
        general.quit_key = 0x79;//VK_F10;
        general.use_hardware_cursor = false;

        video.driver_name = u8"Direct3D";
        video.presentation_mode = Video::PresentationMode::BorderlessFullscreen;
        video.back_buffer_width = 0;
        video.back_buffer_height = 0;
        video.back_buffer_count = 2;
        video.refresh_rate = 0;
        video.vsync_enabled = false;
        video.allow_tearing = true;
    }



} // namespace xl7
