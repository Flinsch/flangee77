#include "DriverResolver.h"

#include <CoreLabs/text/metrics.h>
#include <CoreLabs/text/transform.h>



namespace xl7 {



    /**
     * Returns the driver name of the specified entries that most closely matches
     * the requested configuration name. A name is always returned; in case of
     * doubt, the first entry wins.
     */
    cl7::u8string DriverResolver::resolve_driver_name(const DriverEntries& driver_entries, cl7::u8string_view config_name)
    {
        assert(!driver_entries.empty());

        struct MatchResult
        {
            size_t index;
            int score;
        };

        const cl7::u8string config_lower = cl7::text::transform::to_lower_ascii(config_name);
        std::vector<MatchResult> candidates;

        for (size_t i = 0; i < driver_entries.size(); ++i)
        {
            assert(!driver_entries[i].name.empty());

            const cl7::u8string_view driver_name = driver_entries[i].name;
            const cl7::u8string driver_lower = cl7::text::transform::to_lower_ascii(driver_name);

            // Direct match
            if (config_lower == driver_lower)
            {
                candidates.push_back({.index = i, .score = 1000});
                continue;
            }

            // Alias match
            for (const auto& alias : driver_entries[i].aliases)
            {
                assert(!alias.empty());

                if (config_lower == cl7::text::transform::to_lower_ascii(alias))
                {
                    candidates.push_back({.index = i, .score = 900});
                    break;
                }
            } // for each driver alias

            // Prefix match
            if (driver_lower.starts_with(config_lower))
            {
                candidates.push_back({.index = i, .score = 800});
            }

            // Fuzzy match
            const float similarity = 1.0f - cl7::text::metrics::levenshtein_normalized(config_lower, driver_lower);
            const int score = static_cast<int>(similarity * 700.0f);
            if (score >= 350)
            {
                candidates.push_back({.index = i, .score = score});
            }
        } // for each driver entry

        // In case of doubt, the first entry wins.
        if (candidates.empty())
            return driver_entries[0].name;

        auto best = std::ranges::max_element(candidates, [](const auto& a, const auto& b) {
            if (a.score < b.score) return true;
            if (a.score > b.score) return false;
            return a.index > b.index;
        });

        assert(best != candidates.end());
        assert(best->index < driver_entries.size());

        return driver_entries[best->index].name;
    }



} // namespace xl7
