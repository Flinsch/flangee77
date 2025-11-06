#ifndef DL7_TOML_TYPES_H
#define DL7_TOML_TYPES_H

#include <CoreLabs/ordered_map.h>

#include <CoreLabs/string.h>

#include <chrono>
#include <vector>



namespace dl7::toml {



class Value;

using string_t = cl7::u8string;
using integer_t = int64_t;
using float_t = double;
using boolean_t = bool;
using offset_date_time_t = std::pair<std::chrono::sys_time<std::chrono::seconds>, int16_t>;
using local_date_time_t = std::chrono::local_time<std::chrono::seconds>;
using local_date_t = std::chrono::year_month_day;
using local_time_t = std::chrono::hh_mm_ss<std::chrono::seconds>;
using array_t = std::vector<Value>;
using table_t = cl7::ordered_map<cl7::u8string, Value, cl7::u8string_view>;



} // namespace dl7::toml

#endif // DL7_TOML_TYPES_H
