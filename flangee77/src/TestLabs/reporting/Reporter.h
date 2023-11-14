#pragma once
#ifndef TL7_REPORTING_REPORTER_H
#define TL7_REPORTING_REPORTER_H

#include "./IListener.h"

#include "../Meta.h"
#include "../Result.h"
#include "../Stats.h"

#include <vector>



namespace tl7 {
namespace reporting {



class Reporter
{

private:
    struct ListenerEntry
    {
        signed priority;
        unsigned consecutive_number;
        reporting::IListener* listener;

        auto operator <=> (const ListenerEntry& rhs) const;
    }; // struct ListenerEntry



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The sequential number assigned to newly registered listeners.
     */
    static unsigned consecutive_number;

private:
    /**
     * The registered listeners.
     */
    std::vector<ListenerEntry> _listeners;



    // #############################################################################
    // Listener Management
    // #############################################################################
public:
    /**
     * (Re-)adds the specified listener.
     */
    void add_listener(signed priority, reporting::IListener* listener);

    /**
     * Removes the specified listener.
     */
    void remove_listener(reporting::IListener *listener);



    // #############################################################################
    // Posting Results/Stats
    // #############################################################################
public:
    /**
     * Posts the start of a new test run by notifying registered listeners,
     * providing the total number of test cases (multiple executions/branches due to
     * subcases etc. are not taken into account here).
     */
    void post_start_run(size_t count);

    /**
     * Posts the start of the specified test case by notifying registered listeners.
     */
    void post_start_case(const Meta& meta);

    /**
     * Posts the specified result by notifying registered listeners.
     */
    void post_result(const Result& result);

    /**
     * Posts the specified (test case) stats at the end of a test case by notifying
     * registered listeners.
     */
    void post_end_case(const Stats& stats);

    /**
     * Posts the specified (final) stats at the end of a test run by notifying
     * registered listeners.
     */
    void post_end_run(const Stats& stats);

}; // class Reporter



} // namespace reporting
} // namespace tl7

#endif // TL7_REPORTING_REPORTER_H
