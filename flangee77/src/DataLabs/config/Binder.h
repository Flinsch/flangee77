#ifndef DL7_CONFIG_BINDER_H
#define DL7_CONFIG_BINDER_H

#include "./Value.h"
#include "./Binding.h"

#include <DataLabs/syntax/Diagnostics.h>

#include <concepts>
#include <tuple>
#include <utility>
#include <vector>



namespace dl7::config {



namespace detail {

    template <typename T>
    constexpr bool always_false = false;

    template <typename T>
    struct is_bound_sequence : std::false_type {};

    template <typename T, typename Tallocator>
    struct is_bound_sequence<std::vector<T, Tallocator>> : std::true_type {};

    /**
     * Satisfied by the containers a sequence is read into and written from. A
     * string is one too, technically, which is why it is dealt with first.
     */
    template <typename T>
    concept bound_sequence = is_bound_sequence<T>::value;

} // namespace detail



/**
 * Reads configuration values into native structures and writes them back out, both
 * driven by the same `Binding` description.
 *
 * Reading lays a configuration over what the structure already holds, which is what
 * makes a structure's own defaults the defaults of the configuration: a field the
 * value does not mention keeps what it had, silently, that being the normal case
 * for a configuration file that only states what differs. What does get reported is
 * a key no field answers to (a typo, most likely) and a value a field cannot take
 * (an error, the field keeping its default).
 *
 * Scalars are read leniently where a format leaves no choice: INI and XML have no
 * types to speak of and hand everything over as a string, so a string that reads as
 * a number or a boolean is accepted for a field of that type.
 */
class Binder
{

public:
    /**
     * Reads the given configuration value into the given structure, leaving every
     * field the value does not mention as it was. Whatever there is to complain
     * about ends up in the diagnostics, which are cleared beforehand.
     */
    template <bound_struct Tstruct>
    void load(const Value& value, Tstruct& target)
    {
        _diagnostics.clear();
        _load_struct(value, target, {});
    }

    /**
     * Builds a configuration value from the given structure. Whatever there is to
     * complain about ends up in the diagnostics, which are cleared beforehand.
     */
    template <bound_struct Tstruct>
    Value save(const Tstruct& source)
    {
        _diagnostics.clear();
        return _save_struct(source);
    }

    /**
     * Returns the diagnostics of the most recent `load` or `save` call.
     */
    const syntax::Diagnostics& get_diagnostics() const noexcept { return _diagnostics; }



private:

    // #############################################################################
    // Reading
    // #############################################################################

    template <bound_struct Tstruct>
    void _load_struct(const Value& value, Tstruct& target, const string_t& path)
    {
        if (!value.is_mapping())
        {
            _error(u8"A mapping is expected here.", path);
            return;
        }

        const mapping_t& mapping = value.as_mapping();

        std::apply([&](const auto&... fields)
        {
            (_load_field(mapping, target, fields, path), ...);
        }, Binding<Tstruct>::fields);

        _report_unknown_keys<Tstruct>(mapping, path);
    }

    template <typename Tstruct, typename Tfield>
    void _load_field(const mapping_t& mapping, Tstruct& target, const Tfield& field, const string_t& path)
    {
        const auto it = mapping.find(field.key);

        // A field the value does not mention keeps what it had.
        if (it == mapping.end())
            return;

        _load_value(it->second, target.*(field.member), _extend(path, field.key));
    }

    template <typename Tstruct>
    void _report_unknown_keys(const mapping_t& mapping, const string_t& path)
    {
        for (const auto& p : mapping)
        {
            bool known = false;

            std::apply([&](const auto&... fields)
            {
                ((known = known || fields.key == p.first), ...);
            }, Binding<Tstruct>::fields);

            if (!known)
                _warning(u8"Unknown key.", _extend(path, p.first));
        }
    }

    template <typename T>
    void _load_value(const Value& value, T& target, const string_t& path)
    {
        if constexpr (bound_struct<T>)
            _load_struct(value, target, path);
        else if constexpr (named_enum<T>)
            _load_enum(value, target, path);
        else if constexpr (std::same_as<T, string_t>)
            _load_string(value, target, path);
        else if constexpr (detail::bound_sequence<T>)
            _load_sequence(value, target, path);
        else if constexpr (std::same_as<T, bool>)
            _load_boolean(value, target, path);
        else if constexpr (std::integral<T>)
            _load_integer(value, target, path);
        else if constexpr (std::floating_point<T>)
            _load_float(value, target, path);
        else
            static_assert(detail::always_false<T>, "This type cannot be bound: give it a `Binding` or an `EnumNames` specialization.");
    }

    template <named_enum Tenum>
    void _load_enum(const Value& value, Tenum& target, const string_t& path)
    {
        if (!value.is_string())
        {
            _error(u8"A name is expected here.", path);
            return;
        }

        bool found = false;

        std::apply([&](const auto&... names)
        {
            ((found || (names.name == value.as_string() ? (target = names.value, found = true) : false)), ...);
        }, EnumNames<Tenum>::names);

        if (!found)
            _error(u8"Unknown name.", path);
    }

    void _load_string(const Value& value, string_t& target, const string_t& path);
    void _load_boolean(const Value& value, bool& target, const string_t& path);

    template <std::integral Tinteger>
    void _load_integer(const Value& value, Tinteger& target, const string_t& path)
    {
        integer_t integer = 0;
        if (!_read_integer(value, integer, path))
            return;

        if (!std::in_range<Tinteger>(integer))
        {
            _error(u8"This number is out of the field's range.", path);
            return;
        }

        target = static_cast<Tinteger>(integer);
    }

    template <std::floating_point Tfloat>
    void _load_float(const Value& value, Tfloat& target, const string_t& path)
    {
        float_t number = 0.0;
        if (_read_float(value, number, path))
            target = static_cast<Tfloat>(number);
    }

    template <detail::bound_sequence Tsequence>
    void _load_sequence(const Value& value, Tsequence& target, const string_t& path)
    {
        if (!value.is_sequence())
        {
            _error(u8"A sequence is expected here.", path);
            return;
        }

        // A sequence is replaced rather than added to: a configuration stating a
        // list means that list, not that list on top of the default one.
        target.clear();

        const sequence_t& sequence = value.as_sequence();

        for (size_t i = 0; i < sequence.size(); ++i)
        {
            typename Tsequence::value_type entry{};
            _load_value(sequence[i], entry, _index(path, i));
            target.push_back(std::move(entry));
        }
    }

    /**
     * Reads a scalar as an integer, accepting a string that reads as one. Returns
     * false (having reported) if the value is neither.
     */
    bool _read_integer(const Value& value, integer_t& target, const string_t& path);

    /**
     * Reads a scalar as a floating-point number, accepting a whole number as well
     * as a string that reads as one. Returns false (having reported) if the value is
     * none of those.
     */
    bool _read_float(const Value& value, float_t& target, const string_t& path);



    // #############################################################################
    // Writing
    // #############################################################################

    template <bound_struct Tstruct>
    Value _save_struct(const Tstruct& source)
    {
        mapping_t mapping;

        std::apply([&](const auto&... fields)
        {
            ((mapping[string_t{fields.key}] = _save_value(source.*(fields.member))), ...);
        }, Binding<Tstruct>::fields);

        return Value{std::move(mapping)};
    }

    template <typename T>
    Value _save_value(const T& source)
    {
        if constexpr (bound_struct<T>)
            return _save_struct(source);
        else if constexpr (named_enum<T>)
            return _save_enum(source);
        else if constexpr (std::same_as<T, string_t>)
            return Value{source};
        else if constexpr (detail::bound_sequence<T>)
            return _save_sequence(source);
        else if constexpr (std::same_as<T, bool>)
            return Value{source};
        else if constexpr (std::integral<T>)
            return Value{static_cast<integer_t>(source)};
        else if constexpr (std::floating_point<T>)
            return Value{static_cast<float_t>(source)};
        else
            static_assert(detail::always_false<T>, "This type cannot be bound: give it a `Binding` or an `EnumNames` specialization.");
        return {};
    }

    template <named_enum Tenum>
    Value _save_enum(const Tenum& source)
    {
        bool found = false;
        cl7::u8string_view name;

        std::apply([&](const auto&... names)
        {
            ((found || (names.value == source ? (name = names.name, found = true) : false)), ...);
        }, EnumNames<Tenum>::names);

        if (!found)
        {
            _error(u8"This enumeration value has no name to write.", {});
            return {};
        }

        return Value{name};
    }

    template <detail::bound_sequence Tsequence>
    Value _save_sequence(const Tsequence& source)
    {
        sequence_t sequence;
        sequence.reserve(source.size());

        for (const auto& entry : source)
            sequence.push_back(_save_value(entry));

        return Value{std::move(sequence)};
    }



    // #############################################################################
    // Diagnostics
    // #############################################################################

    /**
     * Extends a field path by one key, which is what the diagnostics point at: the
     * readers' trees hold no source positions, so there is no line to point at
     * instead.
     */
    static string_t _extend(const string_t& path, cl7::u8string_view key);

    /**
     * Extends a field path by one sequence index.
     */
    static string_t _index(const string_t& path, size_t index);

    void _error(cl7::u8string_view message, const string_t& path);
    void _warning(cl7::u8string_view message, const string_t& path);



    syntax::Diagnostics _diagnostics;

}; // class Binder



} // namespace dl7::config

#endif // DL7_CONFIG_BINDER_H
