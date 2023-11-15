#pragma once
#ifndef TL7_SIGNATURE_H
#define TL7_SIGNATURE_H

#include <CoreLabs/string.h>



namespace tl7 {



/**
 * (Unique) signature of any test incident (e.g., registered test case, failed check,
 * warning, exception, etc.).
 */
struct Signature
{

    /** The path to the file where the test incident occurred. */
    const cl7::char_type* file_path;
    /** The line number where the test incident occurred. */
    unsigned line_number;
    /** The iteration number/index (or -1 if not applicable). */
    signed iteration_number;



    Signature(const cl7::char_type* file_path, unsigned line_number, signed iteration_number = -1)
        : file_path( file_path )
        , line_number( line_number )
        , iteration_number( iteration_number )
    {}



    bool operator == (const Signature& rhs) const;
    auto operator <=> (const Signature& rhs) const = default;



    struct hash
    {
        size_t operator () (const Signature& sig) const;
    };

    struct equal_to
    {
        bool operator () (const Signature& sig1, const Signature& sig2) const;
    };

}; // struct Signature



} // namespace tl7

#endif // TL7_SIGNATURE_H
