#ifndef CL7_LOGGING_ABSTRACTLOGHANDLER_H
#define CL7_LOGGING_ABSTRACTLOGHANDLER_H

#include "./LogEntry.h"



namespace cl7::logging {



class AbstractLogHandler
{

public:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    AbstractLogHandler() = default;

    AbstractLogHandler(const AbstractLogHandler&) = delete;
    AbstractLogHandler& operator=(const AbstractLogHandler&) = delete;
    AbstractLogHandler(AbstractLogHandler&&) = delete;
    AbstractLogHandler& operator=(AbstractLogHandler&&) = delete;

    virtual ~AbstractLogHandler() noexcept = default;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Logs the specified entry.
     */
    void on_log(const LogEntry& log_entry);



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    virtual void _write(const LogEntry& log_entry) = 0;

}; // class AbstractLogHandler



} // namespace cl7::logging


#endif // CL7_LOGGING_ABSTRACTLOGHANDLER_H
