#pragma once
#ifndef CL7_LOGGING_COUTLOGHANDLER_H
#define CL7_LOGGING_COUTLOGHANDLER_H
#include "./AbstractLogHandler.h"



namespace cl7 {
namespace logging {



class CoutLogHandler
    : public AbstractLogHandler
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    CoutLogHandler(void) = default;

    /**
     * Destructor.
     */
    virtual ~CoutLogHandler(void) = default;

private:
    /** Copy constructor. */
    CoutLogHandler(const CoutLogHandler&) = delete;
    /** Copy assignment operator. */
    CoutLogHandler& operator = (const CoutLogHandler&) = delete;



    // #############################################################################
    // Implementations
    // #############################################################################
private:
    /**
     * Writes the specified log entry.
     */
    virtual void _write(const LogEntry& log_entry) override;

}; // class AbstractLogHandler



} // namespace logging
} // namespace cl7

#endif // CL7_LOGGING_COUTLOGHANDLER_H
