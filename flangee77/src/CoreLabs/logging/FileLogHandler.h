#pragma once
#ifndef CL7_LOGGING_FILELOGHANDLER_H
#define CL7_LOGGING_FILELOGHANDLER_H
#include "./AbstractLogHandler.h"



namespace cl7::logging {



class FileLogHandler
    : public AbstractLogHandler
{

public:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    FileLogHandler();

    FileLogHandler(const FileLogHandler&) = delete;
    FileLogHandler& operator = (const FileLogHandler&) = delete;
    FileLogHandler(FileLogHandler&&) = delete;
    FileLogHandler& operator = (FileLogHandler&&) = delete;

    ~FileLogHandler() override = default;



private:

    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void _write(const LogEntry& log_entry) override;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Writes the specified "raw" ASCII or UTF-8 encoded std::string to the log
     * file. If specified, the file is initially truncated.
     */
    static void _write_line(std::string_view line, bool truncate = false);

}; // class FileLogHandler



} // namespace cl7::logging

#endif // CL7_LOGGING_FILELOGHANDLER_H
