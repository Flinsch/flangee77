#include "Registry.h"

#include "../logging/StandardLogger.h"

#include <CoreLabs/strings.h>
#include <CoreLabs/sstream.h>



namespace cl7::profiling {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * "Closes" the current frame by updating the samples accordingly, implicitly
     * starting a new frame.
     */
    void Registry::end_frame_and_update_stats(unsigned frame_usecs)
    {
        for (auto& p : _sample_lookup)
            p.second.update_sample(frame_usecs);
    }

    /**
     * "Dumps" the profiling/sample stats to the specified logger (NULL: standard
     * logger).
     */
    void Registry::dump(cl7::logging::Logger* logger) const
    {
        if (_samples.empty())
            return;

        if (!logger)
            logger = &logging::StandardLogger::instance();

        std::wostringstream woss;

        (woss << "  Min  ").put(' ');
        while (woss.str().length() % 14 != 0)
            woss.put(' ');
        (woss << "  Avg  ").put(' ');
        while (woss.str().length() % 14 != 0)
            woss.put(' ');
        (woss << "  Max  ").put(' ');
        while (woss.str().length() % 14 != 0)
            woss.put(' ');
        (woss << "  \u2300 time  ").put(' ');
        while (woss.str().length() % 14 != 0)
            woss.put(' ');
        (woss << "  #  ").put(' ');
        while (woss.str().length() % 10 != 0)
            woss.put(' ');

        woss << "  Sample  ";

        std::wstring utfx = woss.str();
        for (auto& ch : utfx)
        {
            if (ch == 0x20) // space
                ch = wchar_t{0xa0}; // no-break space
        }

        cl7::u8string utf8 = cl7::strings::to_utf8(cl7::strings::reinterpret_utfx(utfx));

        logger->log(cl7::logging::LogEntry{.message=utf8, .type=cl7::logging::LogType::Code});
        logger->log(cl7::logging::LogEntry{.message=u8"--------------------------------------------------------------------------------", .type=cl7::logging::LogType::Code});

        for (const auto& p : _sample_lookup)
        {
            const Sample* root = p.second.sample;
            assert(root);
            if (root->parent_sample != nullptr)
                continue;
            _log(logger, root, 0);
        }
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Begins and returns the sample with the specified name.
     */
    Sample* Registry::_begin_sample(const cl7::u8string& sample_name)
    {
        auto it = _sample_lookup.find({sample_name, _last_opened_sample});
        if (it == _sample_lookup.end())
        {
            auto sample_ptr = std::make_unique<Sample>(sample_name, _last_opened_sample);
            _samples.emplace_back(std::move(sample_ptr));
            auto p = _sample_lookup.insert(std::pair<SampleKey, SampleHandler>({sample_name, _last_opened_sample}, {.sample=_samples.back().get()}));
            assert(p.second);
            it = p.first;
        }

        SampleHandler& sample_handler = it->second;

        sample_handler.open_sample();
        ++_open_sample_count;
        _last_opened_sample = sample_handler.sample;

        return _last_opened_sample;
    }

    /**
     * Ends the specified sample.
     */
    void Registry::_end_sample(Sample* sample)
    {
        assert(sample);
        assert(sample == _last_opened_sample);
        assert(_open_sample_count > 0);
        assert(_open_sample_count == sample->parent_count + 1);

        auto it = _sample_lookup.find({sample->name, sample->parent_sample});
        if (it == _sample_lookup.end())
        {
            assert(false);
            return;
        }

        SampleHandler& sample_handler = it->second;

        sample_handler.close_sample();
        --_open_sample_count;
        _last_opened_sample = sample->parent_sample;
    }

    /**
     * 
     */
    void Registry::_log(cl7::logging::Logger* logger, const Sample* sample, unsigned indent) const
    {
        std::wostringstream woss;

        (woss << std::fixed << std::setprecision(2) << std::setw(6) << (sample->stats.min_pct*100.0f) << " %").put(' ');
        while (woss.str().length() % 14 != 0)
            woss.put(' ');
        (woss << std::fixed << std::setprecision(2) << std::setw(6) << (sample->stats.avg_pct*100.0f) << " %").put(' ');
        while (woss.str().length() % 14 != 0)
            woss.put(' ');
        (woss << std::fixed << std::setprecision(2) << std::setw(6) << (sample->stats.max_pct*100.0f) << " %").put(' ');
        while (woss.str().length() % 14 != 0)
            woss.put(' ');
        if (sample->stats.avg_usecs > 1'000'000)
            (woss << std::fixed << std::setprecision(1) << std::setw(6) << (static_cast<float>(sample->stats.avg_usecs / 1000) * 0.001f) << " s").put(' '); // NOLINT(bugprone-integer-division)
        else if (sample->stats.avg_usecs > 1'000)
            (woss << std::fixed << std::setprecision(1) << std::setw(6) << (static_cast<float>(sample->stats.avg_usecs) * 0.001f) << " ms").put(' ');
        else
            (woss << std::fixed << std::setprecision(1) << std::setw(6) << (static_cast<float>(sample->stats.avg_usecs)) << " \u00b5s").put(' ');
        while (woss.str().length() % 14 != 0)
            woss.put(' ');
        (woss << std::fixed << std::setprecision(0) << std::setw(5) << sample->stats.call_count << " ").put(' ');
        while (woss.str().length() % 10 != 0)
            woss.put(' ');

        unsigned c = indent;
        while (c--)
            woss.put(' ');
        woss << cl7::strings::reinterpret_utfx(cl7::strings::to_utfx(sample->name));

        std::wstring utfx = woss.str();
        for (auto& ch : utfx)
        {
            if (ch == 0x20) // space
                ch = wchar_t{0xa0}; // no-break space
        }

        cl7::u8string utf8 = cl7::strings::to_utf8(cl7::strings::reinterpret_utfx(utfx));

        logger->log(cl7::logging::LogEntry{.message=utf8, .type=cl7::logging::LogType::Code});

        for (const auto& p : _sample_lookup)
        {
            const Sample* child = p.second.sample;
            assert(child);
            if (child->parent_sample != sample)
                continue;
            _log(logger, child, indent + 1);
        }
    }



} // namespace cl7::profiling
