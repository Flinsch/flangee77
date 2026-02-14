#ifndef CL7_PLATFORM_FILESYSTEM_H
#define CL7_PLATFORM_FILESYSTEM_H

#include <CoreLabs/string.h>



namespace cl7::platform::filesystem {



    /**
     * Retrieves the directory that contains the executable module file of the
     * current process.
     */
    cl7::u8string get_module_directory();

    /**
     * Returns the working directory, which is typically identical to the initial
     * directory.
     */
    cl7::u8string get_working_directory();

    /**
     * Returns the initial directory (where the execution started).
     */
    cl7::u8string get_initial_directory();

    /**
     * Returns the current directory.
     */
    cl7::u8string get_current_directory();

    /**
     * Returns the user directory.
     * This is not necessarily the user's home directory, but some file system
     * directory that serves as a common repository for application-specific data.
     */
    cl7::u8string get_user_directory();

    /**
     * Returns the system temp directory.
     */
    cl7::u8string get_temp_directory();



} // namespace cl7::platform::filesystem

#endif // CL7_PLATFORM_FILESYSTEM_H
