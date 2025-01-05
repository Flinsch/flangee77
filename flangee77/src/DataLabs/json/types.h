#ifndef DL7_JSON_TYPES_H
#define DL7_JSON_TYPES_H

#include "./object.h"

#include <CoreLabs/string.h>

#include <vector>



namespace dl7::json {



class json;

using null_t = std::nullptr_t;
using object_t = object;
using array_t = std::vector<json>;
using string_t = cl7::u8string;
using number_real_t = double;
using number_integer_t = int64_t;
using number_unsigned_t = uint64_t;
using boolean_t = bool;



} // namespace dl7::json

#endif // DL7_JSON_TYPES_H
