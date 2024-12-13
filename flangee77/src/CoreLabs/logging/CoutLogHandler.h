#pragma once
#ifndef CL7_LOGGING_COUTLOGHANDLER_H
#define CL7_LOGGING_COUTLOGHANDLER_H
#include "./AbstractLogHandler.h"



namespace cl7::logging {



class CoutLogHandler
    : public AbstractLogHandler
{

public:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    CoutLogHandler() = default;

    CoutLogHandler(const CoutLogHandler&) = delete;
    CoutLogHandler& operator = (const CoutLogHandler&) = delete;
    CoutLogHandler(CoutLogHandler&&) = delete;
    CoutLogHandler& operator = (CoutLogHandler&&) = delete;

    ~CoutLogHandler() override = default;



private:

    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void _write(const LogEntry& log_entry) override;

}; // class CoutLogHandler



} // namespace cl7::logging

#endif // CL7_LOGGING_COUTLOGHANDLER_H
