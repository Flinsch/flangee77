#ifndef CL7_UTILITIES_H
#define CL7_UTILITIES_H

#include <CoreLabs/root.h>



namespace cl7 {



    /**
     * Returns the first parameter that, when interpreted as a Boolean, evaluates to
     * a "true-ish" value. Otherwise, the last value is returned.
     */
    template <typename T>
    const T& coalesce(const T& a, const T& b)
    {
        return a ? a : b;
    }



} // namespace cl7

#endif // CL7_UTILITIES_H
