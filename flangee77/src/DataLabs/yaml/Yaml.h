#ifndef DL7_YAML_YAML_H
#define DL7_YAML_YAML_H

#include "./types.h"

#include <variant>



namespace dl7::yaml {



/**
 * Represents a YAML node: either a scalar (null, string, number, boolean) or a
 * collection (mapping or sequence). Unlike TOML, a YAML document may have a node of
 * any type at its root, which is why this class doubles as the document type (like
 * `Json` does for JSON).
 */
class Yaml
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



    Yaml();
    explicit Yaml(null_t);
    explicit Yaml(mapping_t mapping);
    explicit Yaml(sequence_t sequence);
    explicit Yaml(string_t string);
    explicit Yaml(std::basic_string_view<string_t::value_type> string);
    explicit Yaml(const string_t::value_type* string);

    explicit Yaml(float number);
    explicit Yaml(double number);
    explicit Yaml(long double number);

    explicit Yaml(signed number);
    explicit Yaml(signed long number);
    explicit Yaml(signed long long number);

    explicit Yaml(unsigned number);
    explicit Yaml(unsigned long number);
    explicit Yaml(unsigned long long number);

    explicit Yaml(boolean_t boolean);

    explicit Yaml(Type type);

    Yaml(const Yaml& other);
    Yaml& operator=(const Yaml& other);
    Yaml(Yaml&& other) noexcept;
    Yaml& operator=(Yaml&& other) noexcept;

    ~Yaml() noexcept = default;

    void swap(Yaml& other) noexcept;



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
     * Returns true if this YAML node represents null, an empty mapping, an empty
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
     * Serializes this YAML node into a single-line flow-style string representation.
     */
    string_t to_string() const;



    const Yaml& at(size_t index) const;
    Yaml& at(size_t index);

    const Yaml& at(cl7::u8string_view key) const;
    Yaml& at(cl7::u8string_view key);



    Yaml& operator[](size_t index);

    Yaml& operator[](const cl7::u8string& key);
    Yaml& operator[](cl7::u8string&& key);



    bool operator==(const Yaml& other) const;
    bool operator!=(const Yaml& other) const { return !(*this == other); }



private:
    using mapping_ptr_t = std::unique_ptr<mapping_t>;
    using sequence_ptr_t = std::unique_ptr<sequence_t>;

    float_t _as_float() const;
    integer_t _as_integer() const;

    void _set_string(string_t&& string);

    void _set_float(float_t number);
    void _set_integer(integer_t number);

    std::variant<null_t, mapping_ptr_t, sequence_ptr_t, string_t, float_t, integer_t, boolean_t> _value;

}; // class Yaml



} // namespace dl7::yaml

#endif // DL7_YAML_YAML_H
