#ifndef DL7_CONFIG_VALUE_H
#define DL7_CONFIG_VALUE_H

#include "./types.h"

#include <variant>



namespace dl7::config {



/**
 * A configuration value: either a scalar (null, string, number, boolean) or a
 * collection (mapping or sequence).
 *
 * This is the currency between native C++ structures and the file formats they are
 * read from and written to, and its types are deliberately the intersection of what
 * INI, JSON, TOML, XML, and YAML can all express. It is not a format of its own and
 * has no serialization: the formats convert to and from it, the bindings map
 * structures onto it.
 */
class Value
{

public:
    enum struct Type
    {
        Null,
        Mapping,
        Sequence,
        String,
        Float,
        Integer,
        Boolean,
    };



    Value();
    explicit Value(null_t);
    explicit Value(mapping_t mapping);
    explicit Value(sequence_t sequence);
    explicit Value(string_t string);
    explicit Value(std::basic_string_view<string_t::value_type> string);
    explicit Value(const string_t::value_type* string);

    explicit Value(float number);
    explicit Value(double number);
    explicit Value(long double number);

    explicit Value(signed number);
    explicit Value(signed long number);
    explicit Value(signed long long number);

    explicit Value(unsigned number);
    explicit Value(unsigned long number);
    explicit Value(unsigned long long number);

    explicit Value(boolean_t boolean);

    explicit Value(Type type);

    Value(const Value& other);
    Value& operator=(const Value& other);
    Value(Value&& other) noexcept;
    Value& operator=(Value&& other) noexcept;

    ~Value() noexcept = default;

    void swap(Value& other) noexcept;



    Type get_type() const noexcept { return static_cast<Type>(_value.index()); }

    bool is_null() const noexcept { return std::holds_alternative<null_t>(_value); }
    bool is_mapping() const noexcept { return std::holds_alternative<mapping_ptr_t>(_value); }
    bool is_sequence() const noexcept { return std::holds_alternative<sequence_ptr_t>(_value); }
    bool is_string() const noexcept { return std::holds_alternative<string_t>(_value); }
    bool is_float() const noexcept { return std::holds_alternative<float_t>(_value); }
    bool is_integer() const noexcept { return std::holds_alternative<integer_t>(_value); }
    bool is_number() const noexcept { return is_float() || is_integer(); }
    bool is_boolean() const noexcept { return std::holds_alternative<boolean_t>(_value); }
    bool is_true() const noexcept { const boolean_t* b = std::get_if<boolean_t>(&_value); return b ? *b : false; }
    bool is_false() const noexcept { const boolean_t* b = std::get_if<boolean_t>(&_value); return b ? !*b : false; }
    bool is_primitive() const noexcept { return is_null() || is_string() || is_number() || is_boolean(); }
    bool is_structured() const noexcept { return is_mapping() || is_sequence(); }

    /**
     * Returns true if this value represents null, an empty mapping, an empty
     * sequence, or an empty string; returns false otherwise (i.e., numbers,
     * including 0, and booleans, including false, are considered non-empty).
     */
    bool is_empty() const noexcept;



    const mapping_t& as_mapping() const;
    mapping_t& as_mapping();

    const sequence_t& as_sequence() const;
    sequence_t& as_sequence();

    const string_t& as_string() const;
    string_t& as_string();

    float_t as_float() const;
    integer_t as_integer() const;

    template <std::floating_point Tfloat = float>
    Tfloat as_float() const { return static_cast<Tfloat>(_as_float()); }

    template <std::integral Tinteger = signed>
    Tinteger as_integer() const { return static_cast<Tinteger>(_as_integer()); }

    template <typename Tnumber>
        requires(std::is_arithmetic_v<Tnumber>)
    Tnumber as_number() const
    {
        switch (get_type())
        {
        case Type::Integer:
            return static_cast<Tnumber>(_as_integer());
        default:
            return static_cast<Tnumber>(_as_float());
        }
    }

    boolean_t as_boolean() const;



    void set_null();
    void set_mapping(mapping_t mapping);
    void set_sequence(sequence_t sequence);

    template <cl7::string_constructible<string_t> Tstring>
    void set_string(Tstring&& string) { _set_string(string_t(std::forward<Tstring>(string))); }

    template <std::convertible_to<float_t> Tnumber>
    void set_float(Tnumber number) { _set_float(static_cast<float_t>(number)); }

    template <std::convertible_to<integer_t> Tnumber>
    void set_integer(Tnumber number) { _set_integer(static_cast<integer_t>(number)); }

    template <std::floating_point Tnumber>
    void set_number(Tnumber number) { _set_float(static_cast<float_t>(number)); }

    template <std::integral Tnumber>
    void set_number(Tnumber number) { _set_integer(static_cast<integer_t>(number)); }

    void set_boolean(boolean_t boolean);

    void reset_type(Type type);



    /**
     * Overlays the given value onto this one, which is how a configuration read
     * from a file is laid over the defaults it may leave out. Two mappings merge
     * key by key, recursively. Anything else the overlay has it simply wins with,
     * a sequence being replaced rather than appended to and an explicit null
     * counting as a value like any other.
     */
    void merge(const Value& overlay);



    const Value& at(size_t index) const;
    Value& at(size_t index);

    const Value& at(cl7::u8string_view key) const;
    Value& at(cl7::u8string_view key);



    Value& operator[](size_t index);

    Value& operator[](const cl7::u8string& key);
    Value& operator[](cl7::u8string&& key);



    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const { return !(*this == other); }



private:
    using mapping_ptr_t = std::unique_ptr<mapping_t>;
    using sequence_ptr_t = std::unique_ptr<sequence_t>;

    float_t _as_float() const;
    integer_t _as_integer() const;

    void _set_string(string_t&& string);

    void _set_float(float_t number);
    void _set_integer(integer_t number);

    std::variant<null_t, mapping_ptr_t, sequence_ptr_t, string_t, float_t, integer_t, boolean_t> _value;

}; // class Value



} // namespace dl7::config

#endif // DL7_CONFIG_VALUE_H
