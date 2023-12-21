#pragma once
#ifndef XL7_GRAPHICS_STATES_ABSTRACTSTATES_H
#define XL7_GRAPHICS_STATES_ABSTRACTSTATES_H

#include <CoreLabs/root.h>

#include <array>
#include <vector>



namespace xl7 {
namespace graphics {
namespace states {



class AbstractStates
{

protected:
    template <typename TValue>
    class SingleState
    {
        bool _is_set = false;
        TValue _value;

    public:
        bool is_set() const { return _is_set; }
        TValue get_value(TValue default_value = TValue()) const { return _is_set ? _value : default_value; }
        void set_value(TValue value) { _value = value; _is_set = true; }
        void reset_value() { _is_set = false; }
    }; // class SingleState

    template <typename TProxy, size_t N>
    class StateArray
    {
        struct Entry
        {
            bool is_set = false;
            TProxy value;
        };
        std::array<Entry, N> a;

    public:
        bool is_set(size_t i) const { return a[i].is_set; }
        template <typename TValue = TProxy>
        TValue get_value(size_t i, TValue default_value = TValue()) const { return a[i].is_set ? TValue(a[i].value) : default_value; }
        template <typename TValue = TProxy>
        void set_value(size_t i, TValue value) { a[i] = { true, TProxy(value) }; }
        void reset_value(size_t i) { a[i].is_set = false; }
    }; // class StateArray

    /*template <typename TProxy>
    struct StateVector
    {
        struct Entry
        {
            size_t i;
            TProxy value;

            auto operator <=> (size_t i) const { return this->i <=> i; }
        };
        std::vector<Entry> v;

    public:
        bool is_set(size_t i) const { auto it = std::lower_bound( v.begin(), v.end(), i ); return it != v.end() && it->i == i; }
        template <typename TValue = TProxy>
        TValue get_value(size_t i, TValue default_value = TValue()) const { auto it = std::lower_bound( v.begin(), v.end(), i ); return it != v.end() && it->i == i ? TValue(it->value) : default_value; }
        template <typename TValue = TProxy>
        void set_value(size_t i, TValue value) { auto it = std::lower_bound( v.begin(), v.end(), i ); if ( it != v.end() && it->i == i ) it->value = TProxy(value); else v.insert( it, { i, TProxy(value) } ); }
        void reset_value(size_t i) { auto it = std::lower_bound( v.begin(), v.end(), i ); if ( it != v.end() && it->i == i ) v.erase( it ); }
    }; // class StateVector*/

}; // class AbstractStates



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_ABSTRACTSTATES_H
