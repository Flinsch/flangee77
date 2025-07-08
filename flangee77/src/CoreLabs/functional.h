#ifndef CL7_FUNCTIONAL_H
#define CL7_FUNCTIONAL_H

#include <CoreLabs/root.h>

#include <functional>



namespace cl7 {



    template <typename F, typename T>
    concept unary_function = std::invocable<F, T> && std::convertible_to<std::invoke_result_t<F, T>, T>;



} // namespace cl7

#endif // CL7_FUNCTIONAL_H
