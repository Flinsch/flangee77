#include "DateTimeMatcher.h"



namespace dl7::toml::detail {



    static bool _is_digit(cl7::u8char_t ch)
    {
        return ch >= u8'0' && ch <= u8'9';
    }

    static bool _has_digits(cl7::u8string_view source, size_t pos, size_t count)
    {
        if (pos + count > source.size())
            return false;
        for (size_t i = 0; i < count; ++i)
            if (!_is_digit(source[pos + i]))
                return false;
        return true;
    }

    /** Matches `YYYY-MM-DD` at pos; returns 10 or 0. */
    static size_t _match_date(cl7::u8string_view source, size_t pos)
    {
        if (!_has_digits(source, pos, 4)) return 0;
        if (pos + 10 > source.size()) return 0;
        if (source[pos + 4] != u8'-' || source[pos + 7] != u8'-') return 0;
        if (!_has_digits(source, pos + 5, 2) || !_has_digits(source, pos + 8, 2)) return 0;
        return 10;
    }

    /** Matches `HH:MM:SS` with optional `.fraction` at pos; returns length or 0. */
    static size_t _match_time(cl7::u8string_view source, size_t pos)
    {
        if (pos + 8 > source.size()) return 0;
        if (!_has_digits(source, pos, 2)) return 0;
        if (source[pos + 2] != u8':' || source[pos + 5] != u8':') return 0;
        if (!_has_digits(source, pos + 3, 2) || !_has_digits(source, pos + 6, 2)) return 0;

        size_t len = 8;
        if (pos + len < source.size() && source[pos + len] == u8'.')
        {
            size_t frac = pos + len + 1;
            size_t start = frac;
            while (frac < source.size() && _is_digit(source[frac]))
                ++frac;
            if (frac == start) return len; // dot without digits is not part of the time
            len = frac - pos;
        }

        return len;
    }

    /** Matches a time offset (`Z`/`z` or `(+|-)HH:MM`) at pos; returns length or 0. */
    static size_t _match_offset(cl7::u8string_view source, size_t pos)
    {
        if (pos >= source.size()) return 0;
        if (source[pos] == u8'Z' || source[pos] == u8'z') return 1;
        if (source[pos] != u8'+' && source[pos] != u8'-') return 0;
        if (!_has_digits(source, pos + 1, 2)) return 0;
        if (pos + 6 > source.size() || source[pos + 3] != u8':') return 0;
        if (!_has_digits(source, pos + 4, 2)) return 0;
        return 6;
    }



    size_t DateTimeMatcher::operator()(cl7::u8string_view source) const
    {
        const size_t date_len = _match_date(source, 0);
        if (date_len > 0)
        {
            size_t i = date_len;

            if (i < source.size() && (source[i] == u8'T' || source[i] == u8't' || source[i] == u8' '))
            {
                const size_t time_len = _match_time(source, i + 1);
                if (time_len > 0)
                {
                    size_t j = i + 1 + time_len;
                    return j + _match_offset(source, j);
                }
            }

            // Just a local date (a dangling separator, if any, is left unconsumed).
            return date_len;
        }

        // A standalone local time (no offset).
        return _match_time(source, 0);
    }



} // namespace dl7::toml::detail
