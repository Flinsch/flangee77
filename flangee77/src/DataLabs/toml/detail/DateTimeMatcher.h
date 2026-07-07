#ifndef DL7_TOML_DETAIL_DATETIMEMATCHER_H
#define DL7_TOML_DETAIL_DATETIMEMATCHER_H

#include <CoreLabs/string.h>



namespace dl7::toml::detail {



/**
 * Matches a TOML date-time value (RFC 3339): an offset date-time, a local date-time,
 * a local date, or a local time. Returns the length of the matched value (or 0). A
 * space separator between the date and time parts (instead of `T`/`t`) is supported.
 */
struct DateTimeMatcher
{
    size_t operator()(cl7::u8string_view source) const;
};



} // namespace dl7::toml::detail

#endif // DL7_TOML_DETAIL_DATETIMEMATCHER_H
