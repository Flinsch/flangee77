#pragma once
#ifndef CL7_UTILITIES_H
#define CL7_UTILITIES_H

#include <CoreLabs/root.h>



namespace cl7 {
namespace utilities {



    template <typename T>
    const T& coalesce(const T& a, const T& b)
    {
        return a ? a : b;
    }

    inline void hash_combine(size_t& hash, size_t value)
    {
        hash ^= value + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }



} // namespace utilities
} // namespace cl7

#endif // CL7_UTILITIES_H
