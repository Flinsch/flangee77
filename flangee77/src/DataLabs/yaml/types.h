#ifndef DL7_YAML_TYPES_H
#define DL7_YAML_TYPES_H

#include <CoreLabs/ordered_map.h>

#include <CoreLabs/string.h>

#include <vector>



namespace dl7::yaml {



class Yaml;

using null_t = std::nullptr_t;
using mapping_t = cl7::ordered_map<cl7::u8string, Yaml, cl7::u8string_view>;
using sequence_t = std::vector<Yaml>;
using string_t = cl7::u8string;
using float_t = double;
using integer_t = int64_t;
using boolean_t = bool;



} // namespace dl7::yaml

#endif // DL7_YAML_TYPES_H
