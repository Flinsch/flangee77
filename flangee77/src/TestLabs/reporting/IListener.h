#pragma once
#ifndef TL7_REPORTING_ILISTENER_H
#define TL7_REPORTING_ILISTENER_H

#include "../Meta.h"
#include "../Result.h"
#include "../Stats.h"



namespace tl7 {
namespace reporting {



class IListener
{
public:
    virtual ~IListener() = default;

    // #############################################################################
    // Prototypes
    // #############################################################################
public:
    /**
     * Handles the start of a new test run, providing the total number of test cases
     * (multiple executions/branches due to subcases etc. are not taken into account
     * here).
     */
    virtual void on_start_run(size_t count) = 0;

    /**
     * Handles the start of the specified test case.
     */
    virtual void on_start_case(const Meta& meta) = 0;

    /**
     * Handles the specified result.
     */
    virtual void on_result(const Result& result) = 0;

    /**
     * Handles the specified (test case) stats at the end of a test case.
     */
    virtual void on_end_case(const Stats& stats) = 0;

    /**
     * Handles the specified (final) stats at the end of a test run.
     */
    virtual void on_end_run(const Stats& stats) = 0;

}; // class IListener



} // namespace reporting
} // namespace tl7

#endif // TL7_REPORTING_ILISTENER_H
