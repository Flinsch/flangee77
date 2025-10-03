#ifndef DL7_INI_TYPES_H
#define DL7_INI_TYPES_H

#include <CoreLabs/string.h>

#include <vector>



namespace dl7::ini {



using undefined_t = std::nullptr_t;
using string_t = cl7::u8string;
using float_t = double;
using integer_t = int64_t;
using boolean_t = bool;



} // namespace dl7::ini

#endif // DL7_INI_TYPES_H
