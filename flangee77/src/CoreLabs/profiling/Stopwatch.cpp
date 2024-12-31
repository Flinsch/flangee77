#include "Stopwatch.h"



namespace cl7::profiling {



    Stopwatch::Stopwatch(bool start)
    {
        if (start)
            this->start();
        else
            this->reset();
    }



    /**
     * Calculates and returns the elapsed time.
     */
    std::chrono::steady_clock::duration Stopwatch::calculate_elapsed_time()
    {
        std::chrono::steady_clock::time_point end;
        if (_is_running)
            end = std::chrono::steady_clock::now();
        else
            end = _end;

        return (end - _start) - _pause;
    }

    /**
     * Calculates and returns the elapsed time, in seconds.
     * Elapsed times less than 1 millisecond or greater than a few hours may result
     * in (significant) rounding errors.
     */
    float Stopwatch::calculate_elapsed_secs()
    {
        auto duration = calculate_elapsed_time();

        auto duration_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        if (duration_microseconds <= 1'000'1000)
            return static_cast<float>(duration_microseconds / 1'000) * 0.001f; // NOLINT(bugprone-integer-division)

        auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        if (duration_milliseconds <= 1'000'000)
            return static_cast<float>(duration_milliseconds) * 0.001f;

        auto duration_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        return static_cast<float>(duration_seconds);
    }

    /**
     * Calculates and returns the elapsed time, in milliseconds.
     * The elapsed time should not exceed 46 days to avoid integer overflow.
     */
    unsigned Stopwatch::calculate_elapsed_msecs()
    {
        auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(calculate_elapsed_time()).count();
        assert(duration_milliseconds <= static_cast<decltype(duration_milliseconds)>(std::numeric_limits<unsigned>::max()));
        return static_cast<unsigned>(duration_milliseconds);
    }

    /**
     * Calculates and returns the elapsed time, in microseconds.
     * The elapsed time should not exceed 1 hour to avoid integer overflow.
     */
    unsigned Stopwatch::calculate_elapsed_usecs()
    {
        auto duration_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(calculate_elapsed_time()).count();
        assert(duration_microseconds <= static_cast<decltype(duration_microseconds)>(std::numeric_limits<unsigned>::max()));
        return static_cast<unsigned>(duration_microseconds);
    }

    /**
     * Resets the stopwatch.
     * If the stopwatch was running, it will be restarted immediately; if it was
     * stopped/paused then not.
     */
    void Stopwatch::reset()
    {
        _start = _end = std::chrono::steady_clock::now();
        _pause = std::chrono::steady_clock::duration::zero();
    }

    /**
     * (Re-)starts the stopwatch.
     * The start point in time will be reset to the current time.
     */
    void Stopwatch::start()
    {
        reset();
        _is_running = true;
    }

    /**
     * Stops/pauses the stopwatch.
     */
    void Stopwatch::stop()
    {
        if (!_is_running)
            return;

        _end = std::chrono::steady_clock::now();
        _is_running = false;
    }

    /**
     * Resumes/starts the stopwatch.
     * The start point in time will not be changed.
     */
    void Stopwatch::resume()
    {
        if (_is_running)
            return;

        _pause += std::chrono::steady_clock::now() - _end;
        _is_running = true;
    }



} // namespace cl7::profiling
