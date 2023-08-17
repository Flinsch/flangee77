#pragma once
#ifndef CL7_LOGGING_ABSTRACTLOGHANDLER_H
#define CL7_LOGGING_ABSTRACTLOGHANDLER_H

#include "./LogEntry.h"

#include <vector>



namespace cl7 {
namespace logging {



class AbstractLogHandler
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    AbstractLogHandler(void) = default;

    /**
     * Destructor.
     */
    virtual ~AbstractLogHandler(void) = default;

private:
    /** Copy constructor. */
    AbstractLogHandler(const AbstractLogHandler&) = delete;
    /** Copy assignment operator. */
    AbstractLogHandler& operator = (const AbstractLogHandler&) = delete;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Logs the specified entry.
     */
    void on_log(const LogEntry& log_entry);



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Writes the specified log entry.
     */
    virtual void _write(const LogEntry& log_entry) = 0;

}; // class AbstractLogHandler



} // namespace logging
} // namespace cl7

#endif // CL7_LOGGING_ABSTRACTLOGHANDLER_H
