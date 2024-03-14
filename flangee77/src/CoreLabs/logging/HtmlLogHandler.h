#pragma once
#ifndef CL7_LOGGING_HTMLLOGHANDLER_H
#define CL7_LOGGING_HTMLLOGHANDLER_H
#include "./AbstractLogHandler.h"



namespace cl7 {
namespace logging {



class HtmlLogHandler
    : public AbstractLogHandler
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    HtmlLogHandler();

    /**
     * Destructor.
     */
    virtual ~HtmlLogHandler() override = default;

private:
    /** Copy constructor. */
    HtmlLogHandler(const HtmlLogHandler&) = delete;
    /** Copy assignment operator. */
    HtmlLogHandler& operator = (const HtmlLogHandler&) = delete;



    // #############################################################################
    // Implementations
    // #############################################################################
private:
    /**
     * Writes the specified log entry.
     */
    virtual void _write(const LogEntry& log_entry) override;

}; // class HtmlLogHandler



} // namespace logging
} // namespace cl7

#endif // CL7_LOGGING_HTMLLOGHANDLER_H
