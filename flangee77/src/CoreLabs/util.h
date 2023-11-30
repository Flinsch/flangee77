#pragma once
#ifndef CL7_UTIL_H
#define CL7_UTIL_H

#include <CoreLabs/root.h>



namespace cl7 {
namespace util {



    template <typename T>
    const T& coalesce(const T& a, const T& b)
    {
        return a ? a : b;
    }



} // namespace util
} // namespace cl7

#endif // CL7_UTIL_H
