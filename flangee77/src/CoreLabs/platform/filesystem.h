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
     */
    cl7::u8string get_user_directory();



} // namespace cl7::platform::filesystem

#endif // CL7_PLATFORM_FILESYSTEM_H
