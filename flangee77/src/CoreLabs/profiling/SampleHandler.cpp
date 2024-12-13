#include "Registry.h"



namespace cl7::profiling {



    void SampleHandler::open_sample()
    {
        assert(sample);
        assert(!sample->frame.is_open);

        sample->frame.is_open = true;
        sample->frame.is_profiled = true;
        ++sample->stats.call_count;

        stopwatch.start();
    }

    void SampleHandler::close_sample()
    {
        assert(sample);
        assert(sample->frame.is_open);

        stopwatch.stop();
        unsigned elapsed_usecs = stopwatch.calculate_elapsed_usecs();

        sample->frame.is_open = false;
        sample->frame.frame_usecs += elapsed_usecs;
    }

    void SampleHandler::update_sample(unsigned frame_usecs) // NOLINT(readability-make-member-function-const)
    {
        assert(sample);
        assert(!sample->frame.is_open);

        if (!sample->frame.is_profiled)
            return;

        const unsigned long long n = sample->stats.data_count++;
        const unsigned long long n1 = n + 1;

        const unsigned sample_usecs = sample->frame.frame_usecs;
        const unsigned long long total_usecs = static_cast<unsigned long long>(sample_usecs) + static_cast<unsigned long long>(sample->stats.avg_usecs) * n;

        sample->stats.avg_usecs = static_cast<unsigned>(total_usecs / n1);
        if (!n || sample_usecs < sample->stats.min_usecs)
            sample->stats.min_usecs = sample_usecs;
        if (!n || sample_usecs > sample->stats.max_usecs)
            sample->stats.max_usecs = sample_usecs;

        const float sample_pct = static_cast<float>(sample_usecs) / static_cast<float>(frame_usecs);
        const double total_pct = static_cast<double>(sample_pct) + static_cast<double>(sample->stats.avg_pct) * static_cast<double>(n);

        sample->stats.avg_pct = static_cast<float>(total_pct / static_cast<double>(n1));
        if (!n || sample_pct < sample->stats.min_pct)
            sample->stats.min_pct = sample_pct;
        if (!n || sample_pct > sample->stats.max_pct)
            sample->stats.max_pct = sample_pct;

        sample->frame.is_profiled = false;
        sample->frame.frame_usecs = 0;
    }



} // namespace cl7::profiling
