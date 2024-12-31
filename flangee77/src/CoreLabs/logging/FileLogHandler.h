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
    FileLogHandler& operator=(const FileLogHandler&) = delete;
    FileLogHandler(FileLogHandler&&) = delete;
    FileLogHandler& operator=(FileLogHandler&&) = delete;

    ~FileLogHandler() noexcept override = default;



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
     * Writes the specified "raw" UTF-8 encoded string to the log file. If
     * specified, the file is initially truncated.
     */
    static void _write_line(cl7::u8string_view line, bool truncate = false);

}; // class FileLogHandler



} // namespace cl7::logging

#endif // CL7_LOGGING_FILELOGHANDLER_H
