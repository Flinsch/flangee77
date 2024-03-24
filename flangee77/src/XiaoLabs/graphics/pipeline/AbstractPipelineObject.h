#pragma once
#ifndef XL7_GRAPHICS_PIPELINE_ABSTRACTPIPELINEOBJECT_H
#define XL7_GRAPHICS_PIPELINE_ABSTRACTPIPELINEOBJECT_H

#include <CoreLabs/root.h>

#include <array>
//#include <vector>



namespace xl7 {
namespace graphics {
namespace pipeline {



class AbstractPipelineObject
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

    template <typename TProxy, unsigned N>
    class StateArray
    {
        struct Entry
        {
            bool is_set = false;
            TProxy value;
        };
        std::array<Entry, N> a;

    public:
        bool is_set(unsigned i) const { return a[i].is_set; }
        template <typename TValue = TProxy>
        TValue get_value(unsigned i, TValue default_value = TValue()) const { return a[i].is_set ? TValue(a[i].value) : default_value; }
        template <typename TValue = TProxy>
        void set_value(unsigned i, TValue value) { a[i] = { true, TProxy(value) }; }
        void reset_value(unsigned i) { a[i].is_set = false; }
    }; // class StateArray

    /*template <typename TProxy>
    struct StateVector
    {
    struct Entry
    {
    unsigned i;
    TProxy value;

    auto operator <=> (unsigned i) const { return this->i <=> i; }
    };
    std::vector<Entry> v;

    public:
    bool is_set(unsigned i) const { auto it = std::lower_bound( v.begin(), v.end(), i ); return it != v.end() && it->i == i; }
    template <typename TValue = TProxy>
    TValue get_value(unsigned i, TValue default_value = TValue()) const { auto it = std::lower_bound( v.begin(), v.end(), i ); return it != v.end() && it->i == i ? TValue(it->value) : default_value; }
    template <typename TValue = TProxy>
    void set_value(unsigned i, TValue value) { auto it = std::lower_bound( v.begin(), v.end(), i ); if ( it != v.end() && it->i == i ) it->value = TProxy(value); else v.insert( it, { i, TProxy(value) } ); }
    void reset_value(unsigned i) { auto it = std::lower_bound( v.begin(), v.end(), i ); if ( it != v.end() && it->i == i ) v.erase( it ); }
    }; // class StateVector*/

#define _XL7_GRAPHICS_PIPELINE_SINGLE_STATE(name, type, default_value, dirty_flag) \
private: \
    SingleState<type> _##name; \
public: \
    bool is_##name##_set() const { return _##name.is_set(); } \
    type get_##name() const { return _##name.get_value( default_value ); } \
    void set_##name(type value) { _##name.set_value( value ); _set_dirty( dirty_flag ); } \
    void reset_##name() { _##name.reset_value(); _unset_dirty( dirty_flag ); }

#define _XL7_GRAPHICS_PIPELINE_STATE_ARRAY(name, count, type, default_value, dirty_flag_base) \
private: \
    StateArray<type, count> _##name; \
public: \
    bool is_##name##_set(unsigned i) const { return i >= count ? false : _##name.is_set( i ); } \
    type get_##name(unsigned i) const { return i >= count ? default_value : _##name.get_value( i, default_value ); } \
    void set_##name(unsigned i, type value) { if (i >= count ) return; _##name.set_value( i, value ); _set_dirty( dirty_flag_base << i ); } \
    void reset_##name(unsigned i) { if (i >= count ) return; _##name.reset_value( i ); _unset_dirty( dirty_flag_base << i ); }

#define _XL7_GRAPHICS_PIPELINE_STATE_ARRAY_DEFAULT0(name, count, type, default_value, dirty_flag_base) \
    _XL7_GRAPHICS_PIPELINE_STATE_ARRAY( name, count, type, default_value, dirty_flag_base ) \
public: \
    bool is_##name##_set() const { return _##name.is_set( 0 ); } \
    type get_##name() const { return _##name.get_value( 0, default_value ); } \
    void set_##name(type value) { _##name.set_value( 0, value ); _set_dirty( dirty_flag_base ); } \
    void reset_##name() { _##name.reset_value( 0 ); _unset_dirty( dirty_flag_base ); }



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * 
     */
    unsigned _dirty_flags;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    bool is_dirty() const { return _dirty_flags != 0; }
    void undirty() { _dirty_flags = 0; }
protected:
    bool _is_dirty(unsigned dirty_flag) const { return (_dirty_flags & dirty_flag) == dirty_flag; }
    void _set_dirty(unsigned dirty_flag) { _dirty_flags |= dirty_flag; }
    void _unset_dirty(unsigned dirty_flag) { _dirty_flags &= ~dirty_flag; }

}; // class AbstractPipelineObject



} // namespace pipeline
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_PIPELINE_ABSTRACTPIPELINEOBJECT_H
