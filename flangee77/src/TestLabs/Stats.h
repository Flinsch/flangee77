#pragma once
#ifndef TL7_STATS_H
#define TL7_STATS_H

#include "./Result.h"



namespace tl7 {



struct Stats
{

    struct Group
    {
        /** The total number. */
        unsigned total_count = 0;
        /** The number that passed. */
        unsigned pass_count = 0;
        /** The number that failed. */
        unsigned fail_count = 0;

        Group operator + (const Group& rhs) const;
        Group& operator += (const Group& rhs);

        void reset();
        void update(bool success);
    };

    /** The test cases executed. */
    Group cases;
    /** The subcase branches traversed. */
    Group subcases;
    /** The checks performed. */
    Group checks;
    /** The assertions made. */
    Group assertions;

    /** The number of unexpected exceptions encountered. */
    unsigned exception_count = 0;
    /** The number of warnings issued. */
    unsigned warning_count = 0;
    /** The execution time in milliseconds. */
    unsigned execution_time_msecs = 0;

    Stats operator + (const Stats& rhs) const;
    Stats& operator += (const Stats& rhs);

    void reset();
    void update(const Result& result);

    unsigned interim_fail_count() const;

}; // struct Stats



} // namespace tl7

#endif // TL7_STATS_H
