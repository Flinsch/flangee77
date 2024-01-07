#pragma once
#ifndef CL7_FILESYSTEM_H
#define CL7_FILESYSTEM_H

#include <CoreLabs/string.h>



namespace cl7 {
namespace filesystem {



    /**
     * Returns the working directory of the module instance.
     */
    cl7::string get_working_directory();

    /**
     * Returns the initial directory (where the execution started).
     */
    cl7::string get_initial_directory();

    /**
     * Returns the current directory.
     */
    cl7::string get_current_directory();

    /**
     * Returns the user directory.
     */
    cl7::string get_user_directory();



} // namespace filesystem
} // namespace cl7

#endif // CL7_FILESYSTEM_H
