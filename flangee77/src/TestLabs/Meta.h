#ifndef TL7_META_H
#define TL7_META_H

#include "./Signature.h"

#include <CoreLabs/string.h>



namespace tl7 {



/**
 * Description of any test incident (e.g., registered test case, failed check,
 * warning, exception, etc.).
 */
struct Meta
    : public Signature
{

    /** The textual description of the test incident. */
    cl7::u8string stringification;



    Meta(cl7::u8string_view stringification, const char* file_path, unsigned line_number, signed iteration_number = -1)
        : Signature{file_path, line_number, iteration_number}
        , stringification(stringification)
    {}



    auto operator<=>(const Meta& rhs) const = default;

}; // struct Meta



} // namespace tl7

#endif // TL7_META_H
