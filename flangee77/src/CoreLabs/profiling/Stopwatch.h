#ifndef CL7_PROFILING_STOPWATCH_H
#define CL7_PROFILING_STOPWATCH_H

#include <CoreLabs/root.h>

#include <chrono>



namespace cl7::profiling {



class Stopwatch
{

public:
    explicit Stopwatch(bool start = true);



    /**
     * Returns the flag that indicates whether the stopwatch is running (true) or
     * stopped/paused (false).
     */
    bool is_running() const { return _is_running; }

    /**
     * Calculates and returns the elapsed time.
     */
    std::chrono::steady_clock::duration calculate_elapsed_time();

    /**
     * Calculates and returns the elapsed time, in seconds.
     * Elapsed times less than 1 millisecond or greater than a few hours may result
     * in (significant) rounding errors.
     */
    float calculate_elapsed_secs();

    /**
     * Calculates and returns the elapsed time, in milliseconds.
     * The elapsed time should not exceed 46 days to avoid integer overflow.
     */
    unsigned calculate_elapsed_msecs();

    /**
     * Calculates and returns the elapsed time, in microseconds.
     * The elapsed time should not exceed 1 hour to avoid integer overflow.
     */
    unsigned calculate_elapsed_usecs();

    /**
     * Resets the stopwatch.
     * If the stopwatch was running, it will be restarted immediately; if it was
     * stopped/paused then not.
     */
    void reset();

    /**
     * (Re-)starts the stopwatch.
     * The start point in time will be reset to the current time.
     */
    void start();

    /**
     * Stops/pauses the stopwatch.
     */
    void stop();

    /**
     * Resumes/starts the stopwatch.
     * The start point in time will not be changed.
     */
    void resume();



private:
    /** The start point in time. */
    std::chrono::steady_clock::time_point _start;
    /** The end point in time. */
    std::chrono::steady_clock::time_point _end;
    /** The cumulative duration of pauses during the stopwatch's operation. */
    std::chrono::steady_clock::duration _pause = {};
    /** The flag that indicates whether the stopwatch is running (true) or stopped/paused (false). */
    bool _is_running = false;

}; // class Stopwatch



} // namespace cl7::profiling

#endif // CL7_PROFILING_STOPWATCH_H
