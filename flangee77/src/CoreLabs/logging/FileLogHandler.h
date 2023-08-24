#pragma once
#ifndef CL7_LOGGING_FILELOGHANDLER_H
#define CL7_LOGGING_FILELOGHANDLER_H
#include "./AbstractLogHandler.h"



namespace cl7 {
namespace logging {



class FileLogHandler
    : public AbstractLogHandler
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    FileLogHandler(void);

    /**
     * Destructor.
     */
    virtual ~FileLogHandler(void) override = default;

private:
    /** Copy constructor. */
    FileLogHandler(const FileLogHandler&) = delete;
    /** Copy assignment operator. */
    FileLogHandler& operator = (const FileLogHandler&) = delete;



    // #############################################################################
    // Implementations
    // #############################################################################
private:
    /**
     * Writes the specified log entry.
     */
    virtual void _write(const LogEntry& log_entry) override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Writes the specified text line to the log file. If specified, the file is
     * initially truncated.
     */
    void _write_line(cl7::string_view line, bool truncate = false);

}; // class AbstractLogHandler



} // namespace logging
} // namespace cl7

#endif // CL7_LOGGING_FILELOGHANDLER_H
