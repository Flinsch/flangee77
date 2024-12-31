#ifndef CL7_FILESYSTEM_H
#define CL7_FILESYSTEM_H

#include <CoreLabs/string.h>



namespace cl7::filesystem {



    /**
     * Returns the working directory of the module instance.
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



} // namespace cl7::filesystem

#endif // CL7_FILESYSTEM_H
