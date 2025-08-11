#ifndef CL7_UTILITIES_H
#define CL7_UTILITIES_H

#include <CoreLabs/root.h>



namespace cl7 {



    /**
     * Returns the first parameter that, when interpreted as a Boolean, evaluates to
     * a "true-ish" value. Otherwise, the last value is returned.
     */
    template <typename T1, typename T2>
        requires(std::same_as<std::decay_t<T1>, std::decay_t<T2>> && std::is_convertible_v<T1, bool>)
    auto coalesce(T1&& a, T2&& b)
    {
        using T = std::decay_t<T1>;
        return static_cast<bool>(a) ? static_cast<T>(std::forward<T1>(a)) : static_cast<T>(std::forward<T2>(b));
    }



} // namespace cl7

#endif // CL7_UTILITIES_H
