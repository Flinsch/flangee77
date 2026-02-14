#ifndef CL7_PLATFORM_DETAIL_WINDOWS_FILESYSTEM_H
#define CL7_PLATFORM_DETAIL_WINDOWS_FILESYSTEM_H
#include <CoreLabs/platform/filesystem.h>
#if F77_IS_WINDOWS



namespace cl7::platform::detail::windows::filesystem {



    cl7::u8string get_module_directory();
    cl7::u8string get_working_directory();
    cl7::u8string get_initial_directory();
    cl7::u8string get_current_directory();
    cl7::u8string get_user_directory();
    cl7::u8string get_temp_directory();



} // namespace cl7::platform::detail::windows::filesystem

#endif // F77_IS_WINDOWS
#endif // CL7_PLATFORM_DETAIL_WINDOWS_FILESYSTEM_H
