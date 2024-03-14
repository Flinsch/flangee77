#pragma once
#ifndef CL7_UTILITIES_H
#define CL7_UTILITIES_H

#include <CoreLabs/root.h>

#include <bit>



namespace cl7 {
namespace utilities {



    /**
     * Returns the first parameter that, when interpreted as a Boolean, evaluates to
     * a "true-ish" value.
     */
    template <typename T>
    const T& coalesce(const T& a, const T& b)
    {
        return a ? a : b;
    }



} // namespace utilities
} // namespace cl7

#endif // CL7_UTILITIES_H
