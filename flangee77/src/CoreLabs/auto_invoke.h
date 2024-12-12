#pragma once
#ifndef CL7_AUTO_INVOKE_H
#define CL7_AUTO_INVOKE_H

#include <CoreLabs/root.h>

#include <utility>



namespace cl7 {



template <class Callable>
class auto_invoke
{

public:
    explicit auto_invoke(const Callable& callable) noexcept : _callable(callable) {}
    explicit auto_invoke(Callable&& callable) noexcept : _callable(std::move(callable)) {}

    auto_invoke(const auto_invoke&) = delete;
    auto_invoke(auto_invoke&& other) noexcept : _callable(std::move(other._callable)), _invoke(std::exchange(other._invoke, false)) {}

    ~auto_invoke() noexcept { if (_invoke) _callable(); }

    auto_invoke& operator = (const auto_invoke&) = delete;
    auto_invoke& operator = (auto_invoke&& other) noexcept { _callable = std::move(other._callable); _invoke = std::exchange(other._invoke, false); return *this; }

    void swap(auto_invoke& other) noexcept { std::swap(_callable, other._callable); std::swap(_invoke, other._invoke); }

    void reset() noexcept { _invoke = false; }
    void reset(Callable&& callable) noexcept { _callable = std::move(callable); _invoke = true; }

private:
    Callable _callable;
    bool _invoke = true;

}; // class auto_invoke



template <class Callable>
auto finally(Callable&& callable) noexcept
{
    return auto_invoke<std::decay_t<Callable>>(std::forward<Callable>(callable));
}



} // namespace cl7

#endif // CL7_AUTO_INVOKE_H
