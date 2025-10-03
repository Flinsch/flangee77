#ifndef DL7_JSON_TYPES_H
#define DL7_JSON_TYPES_H

#include <CoreLabs/ordered_map.h>

#include <CoreLabs/string.h>

#include <vector>



namespace dl7::json {



class Json;

using null_t = std::nullptr_t;
using object_t = cl7::ordered_map<cl7::u8string, Json, cl7::u8string_view>;
using array_t = std::vector<Json>;
using string_t = cl7::u8string;
using float_t = double;
using integer_t = int64_t;
using boolean_t = bool;



} // namespace dl7::json

#endif // DL7_JSON_TYPES_H
