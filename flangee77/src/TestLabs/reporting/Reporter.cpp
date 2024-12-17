#include "Reporter.h"

#include <algorithm>



namespace tl7::reporting {



    auto Reporter::ListenerEntry::operator <=> (const ListenerEntry& rhs) const
    {
        if (auto c = -priority <=> -rhs.priority; c != 0)
            return c;
        return consecutive_number <=> rhs.consecutive_number;
    }



    /**
     * The sequential number assigned to newly registered listeners.
     */
    unsigned Reporter::consecutive_number = 0;



    // #############################################################################
    // Listener Management
    // #############################################################################

    /**
     * (Re-)adds the specified listener.
     */
    void Reporter::add_listener(signed priority, reporting::IListener* listener)
    {
        remove_listener(listener);

        ListenerEntry entry{.priority=priority, .consecutive_number=consecutive_number++, .listener=listener};
        auto it = std::ranges::upper_bound(_listeners, entry, std::less<>());
        _listeners.insert(it, entry);
    }

    /**
     * Removes the specified listener.
     */
    void Reporter::remove_listener(const reporting::IListener* listener)
    {
        for (auto it = _listeners.begin(); it != _listeners.end(); )
        {
            if (it->listener == listener)
                it = _listeners.erase(it);
            else
                ++it;
        } // for each listener
    }



    // #############################################################################
    // Posting Results/Stats
    // #############################################################################

    /**
     * Posts the start of a new test run by notifying registered listeners,
     * providing the total number of test cases (multiple executions/branches due to
     * subcases etc. are not taken into account here).
     */
    void Reporter::post_start_run(size_t count)
    {
        for (const auto& entry : _listeners)
        {
            entry.listener->on_start_run(count);
        } // for each listener
    }

    /**
     * Posts the start of the specified test case by notifying registered listeners.
     */
    void Reporter::post_start_case(const Meta& meta)
    {
        for (const auto& entry : _listeners)
        {
            entry.listener->on_start_case(meta);
        } // for each listener
    }

    /**
     * Posts the specified result by notifying registered listeners.
     */
    void Reporter::post_result(const Result& result)
    {
        for (const auto& entry : _listeners)
        {
            entry.listener->on_result(result);
        } // for each listener
    }

    /**
     * Posts the specified (test case) stats at the end of a test case by notifying
     * registered listeners.
     */
    void Reporter::post_end_case(const Stats& stats)
    {
        for (const auto& entry : _listeners)
        {
            entry.listener->on_end_case(stats);
        } // for each listener
    }

    /**
     * Posts the specified stats by notifying registered listeners.
     */
    void Reporter::post_end_run(const Stats& stats)
    {
        for (const auto& entry : _listeners)
        {
            entry.listener->on_end_run(stats);
        } // for each listener
    }



} // namespace tl7::reporting
